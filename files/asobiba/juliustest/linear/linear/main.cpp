#include <Winsock2.h>
#include <mswsock.h>

#include <windows.h>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../liblinear/linear.h"
#include "LoadLibraryHelper.h"

extern "C"{
	#include <julius/juliuslib.h>
};



Jconf* g_jconfFile;
Recog * g_recogFile;

bool g_OKorBAD;
std::ostream* g_TrainFile;

//plus�̃X�R�A���v�Z���܂��B
float computePlusScore(float cmscore,float score,int sentnum,float mseclen) 
{
	//�܂��́A���̃X�R�A��^�����ԂŊ���܂��B
	float plus_sentence_score = score / mseclen;

	//���ɁA�������ɉ����ăy�i���e�B��^���܂��B
	//�����������Ƃ������Ƃ́A�����悤�Ȃ��̂������Ė����Ă���킯�ŁA����𔽉f����B
	switch(sentnum)
	{
	case 0:
	case 1:
		//�y�i���e�B�Ȃ�
		break;
	case 2:
		plus_sentence_score *= 1.1f;
		break;
	case 3:
		plus_sentence_score *= 1.15f;
		break;
	case 4:
		plus_sentence_score *= 1.5f;
		break;
	case 5:
		plus_sentence_score *= 2.0f;
		break;
	default:
		plus_sentence_score *= 3.0f;
		break;
	}

	//cmscore �̕��ϒl
//	float cmsplus = 0;
//	cmsplus = cmsplus / nodes.size();
//	cmsplus = (1 - cmsplus + 1) * 10; //���]������10�{���ăy�i���e�B�Ƃ���B�傫���قǔ������ł���
//
//	plus_sentence_score *= cmsplus;	//cmscore �ɂ��y�i���e�B������.

	float cmsplus = (1 - cmscore + 1) * 10; //���]������10�{���ăy�i���e�B�Ƃ���B�傫���قǔ������ł���
	plus_sentence_score *= cmsplus;	//cmscore �ɂ��y�i���e�B������.
	

	//�ŁA���̃X�R�A�Ō���΁A
	//�����Ă����܂��s����-20���x�A���s�����-100���x�Ɏ��������ŁA
	//����� 1 - 0 �Ɋۂ߂�ƁA������ۂ�������ł���www
	//
	//�������P������������� 0.80 ���炢�ɂȂ�悤�ɒ������܂��B
	plus_sentence_score = (110 - (plus_sentence_score * -1)) / 100;
	if (plus_sentence_score >= 1) return 1;
	if (plus_sentence_score <= 0) return 0;

	return plus_sentence_score;
}

//�����̐��ɂ��y�i���e�B
//�����������ƁA���ꂾ�����f�ɍ����Ă���Ƃ������ƂŃy�i���e�B��^���܂��B
//�܂��A�S�~�Ƀ}�b�`�����ꍇ�͂��傫�ȃy�i���e�B��^���܂��B
//���������͓K���ł��B�@�B�w�K�Ƃ��������΂����񂾂��ǁB
int countHypothesisPenalty(const Recog *recog) 
{
	float gomiCount = 0;
	for(RecogProcess* r = recog->process_list; r ; r=r->next) 
	{
		//�S�~�������܂��B
		if (! r->live || r->result.status < 0 ) 
		{
			continue;
		}
		//���ꕶ�͂��ǂ����`�F�b�N����
		std::list<int> checkDict;

		auto winfo = r->lm->winfo;
		for(auto n = 0; n < r->result.sentnum; n++) 
		{ // for all sentences
			const auto s = &(r->result.sent[n]);
			const auto seq = s->word;
			const auto seqnum = s->word_num;

			//�J�n�����Ȃ����̂͂���
			if ( strcmp(winfo->woutput[seq[0]]  , "<s>") != 0 )
			{
				gomiCount +=1.0f;
				continue;
			}
			if (seqnum <= 2)
			{//start word end 
				gomiCount +=1.0f;
				continue;
			}
			//�S�~���͂Ȃ�y�i���e�B
			if ( strcmp(winfo->woutput[seq[0]]  , "gomi") == 0 )
			{
				gomiCount +=1.0f;
				continue;
			}
			//�ӂ[�̃R�}���h�� �X�^�[�g���� �Ăт��� �R�}���h �X�^�[�g�����I��� ��4�ȏ�ɂȂ��B
			//�R�}���h�� �G�A�R��(����|������) �̂悤�ɁA���K�\��������� �R�}���h1 �R�}���h2 �ƕ��������B
			int dict;
			if ( seqnum >= 4  )
			{
				//���ꕶ�͂Ȃ�y�i���e�B�͏��Ȃ�����  �G�A�R��(����|������) �ȂǓ��ꕶ��
				dict = atoi(winfo->wname[seq[2]]);
			}
			else
			{
				//�f���ł͎g��Ȃ����e���|�������[���̏ꍇ�ɂ́A�Ăт����������̂ł��̕��Z���Ȃ�B
				dict = atoi(winfo->wname[seq[1]]);
			}
			if (dict <= 3)
			{//�S�~���[��
				gomiCount +=1.0f;
				continue;
			}

			//���łɓo�ꂵ���m�[�h���ǂ�������ׂ�
			if (std::find(checkDict.begin(),checkDict.end() , dict) != checkDict.end()	)
			{
				gomiCount += 0.2f;
				continue;
			}

			//�V�K�̕��͂Ȃ瑽���̃y�i���e�B
			gomiCount += 0.5f;
			checkDict.push_back(dict);
		}
	}
	return (int)(gomiCount + 1.5f);
}

//�ǂ݂��擾����
std::string ConvertYomi(const WORD_INFO * wordinfo,int index) 
{
	std::string yomi;

	char buf[64];
	for (int j=0;j<wordinfo->wlen[index];j++) 
	{
		yomi += center_name( (wordinfo->wseq[index][j]->name),buf);
	}
	return yomi;
}


//julius�F�������f�[�^
void g_output_result(Recog *recog, void *dummy)
{
	//���������Ԃ̑���
	const float mseclen = (float)recog->mfcclist->param->samplenum * (float)recog->jconf->input.period * (float)recog->jconf->input.frameshift / 10000.0f;
	//�����̐��ɂ��y�i���e�B
	const int hypothesisPenalty = countHypothesisPenalty(recog);

	//1:���� 2:���s 
	if (g_OKorBAD)
	{
		*g_TrainFile << "1";
	}
	else
	{
		*g_TrainFile << "2";
	}
	for(const RecogProcess* r = recog->process_list; r ; r=r->next) 
	{
		//�S�~�������܂��B
		if (! r->live || r->result.status < 0 ) 
		{
			continue;
		}

		// output results for all the obtained sentences
		const auto winfo = r->lm->winfo;
		for(auto n = 0; n < r->result.sentnum; n++) 
		{ // for all sentences
			const auto s = &(r->result.sent[n]);
			const auto seq = s->word;
			const auto seqnum = s->word_num;

			int i_seq;
			// output word sequence like Julius 
			//0 , [1 , 2, 3, 4], 5, �Ɛ擪�ƍŌ�������Ă���A�J�n�I�[�A�L���𔲂�����
			int i;
			for(i = 0;i<seqnum;i++)
			{
				//1�P�� --> �P��̏W�������ɂȂ��
				i_seq = seq[i];

				//�J�n�ƏI�����΂�
				if (	strcmp(winfo->woutput[i_seq]  , "<s>") == 0 
					||  strcmp(winfo->woutput[i_seq]  , "</s>") == 0 
				){
					continue;
				}
				break;
			}
			if (i >= seqnum) 
			{
				continue;
			}
			//dict ���� plus����rule �����߂�
			int dict = atoi(winfo->wname[i_seq]);

			//�}�b�`��݂��Ȃ��擾����
			std::string yomi = ConvertYomi(winfo,i_seq);

			//cmscore�̐���(���̂܂܂ł͎g���Ȃ��q���)
			auto cmscore = s->confidence[i];
			//�f��1 cmsscore
			*g_TrainFile << " 1:" << cmscore;

			//julius �̃X�R�A �ޓx�炵���B�}�C�i�X�l�B0�ɋ߂��قǐ������炵���B
			//�u���v�A�ւ�Ă��ȕ��͂����Ă��A�X�R�A�������Ȃ��Ă��܂����A�������͂�����ƃX�R�A����]�I�ɒႭ�Ȃ�
			//�̂ŁA���̂܂܂��Ǝg���Ȃ��B
			auto score = s->score;

			//�f��2 ���̓X�R�A
			*g_TrainFile << " 2:" << score;

			auto all_frame = r->result.num_frame;

			//�f��3 �t���[����
			*g_TrainFile << " 3:" << all_frame;

			//�f��4 �����̐��ɂ��y�i���e�B
			*g_TrainFile << " 4:" << hypothesisPenalty;

			//�f��5 �^������
			*g_TrainFile << " 5:" << mseclen;

			//�����g����X�R�A���v�Z���܂��B
			//			oneSentence->plus_sentence_score = computePlusScore(oneSentence->nodes,s->score,r->result.sentnum,mseclen);
			auto plus_sentence_score = computePlusScore(cmscore,s->score,hypothesisPenalty,mseclen);
			//�f��6 plus�X�R�A
			*g_TrainFile << " 6:" << plus_sentence_score;

			//�f��7 �T���v�����H
			*g_TrainFile << " 7:" << r->lm->am->mfcc->param->header.samplenum;
			//�f��8 �Ȃɂ���
//			*g_TrainFile << " 8:" << r->lm->am->mfcc->param->header.wshift;
			//�f��9 �Ȃɂ���
//			*g_TrainFile << " 9:" << r->lm->am->mfcc->param->veclen;
			//�f��10�` ���ꂪ���߂ĂɂȂ����B
			int feature = 10;
			for(int vecI = 0 ; vecI < r->lm->am->mfcc->param->header.samplenum ;vecI++ )
			{
				for(int vecN = 0 ; vecN < r->lm->am->mfcc->param->veclen ;vecN++ )
				{
					*g_TrainFile << " " << feature++ << ":" << r->lm->am->mfcc->param->parvec[vecI][vecN];
				}
			}
			*g_TrainFile << std::endl;

			return ;
		}
	}

}


//julius �^���J�n
bool JuliusReco(const std::string& fullname)
{
	int ret = j_open_stream(g_recogFile,(char*) fullname.c_str() );
	if(ret < 0)
	{
		return false;
	}
	j_recognize_stream(g_recogFile);
	return true;
}

//julius������
bool JuliusFileStart()
{
	const char* argv[]={
		"juliusplus"
		,"-C"
		,"testfile.jconf"
	};
	int argc = sizeof(argv)/sizeof(argv[0]);

	//julius �� C�֐�������Aconst�O���� char** �ɂ��邵���Ȃ��B
	g_jconfFile = j_config_load_args_new(argc, (char**)argv);
	/* else, you can load configurations from a jconf file */
	//jconf = j_config_load_file_new(jconf_filename);
	if (g_jconfFile == NULL) 
	{
		return false;
	}

	/* 2. create recognition instance according to the jconf */
	/* it loads models, setup final parameters, build lexicon
	and set up work area for recognition */
	g_recogFile = j_create_instance_from_jconf(g_jconfFile);
	if (g_recogFile == NULL)
	{
		return false;
	}

	callback_add(g_recogFile, CALLBACK_RESULT, g_output_result, 0);

	// Initialize audio input
	if (j_adin_init(g_recogFile) == FALSE) 
	{
		return false;
	}
	return true;
}

//�����Ǝ��s�f�[�^�̃f�B���N�g���X�L����
bool ScanDir(const std::string & dir)
{
	//�f�B���N�g���Ɋi�[����Ă�������擾���ă}�b�`���O���Ă����܂��B
	std::string path = dir + "\\*.wav";

	_finddata_t data;
	long handle = _findfirst( path.c_str() , &data );
	if (handle == -1)
	{
		return true;
	}
	do
	{
		const std::string filename = dir + "\\" + data.name;
		if (data.attrib & _A_SUBDIR)
		{
			continue;
		}
		bool r = JuliusReco(filename);
		if (!r)
		{
			//julius��wave�H�ׂ������炦������B�ǂ����悤�E�E�E
		}
	}
	while( ! _findnext(handle,&data) );
	_findclose(handle);

	return true;
}

class XLMachineLearningLibliear
{
public:
	XLMachineLearningLibliear()
	{
		lib.Load("liblinear.dll");
		func_train = (def_train) lib.GetProcAddress("train");
		func_cross_validation = (def_cross_validation) lib.GetProcAddress("cross_validation");

		func_predict_values = (def_predict_values) lib.GetProcAddress("predict_values");
		func_predict = (def_predict) lib.GetProcAddress("predict");
		func_predict_probability = (def_predict_probability) lib.GetProcAddress("predict_probability");

		func_save_model = (def_save_model) lib.GetProcAddress("save_model");
		func_load_model = (def_load_model) lib.GetProcAddress("load_model");

		func_get_nr_feature = (def_get_nr_feature) lib.GetProcAddress("get_nr_feature");
		func_get_nr_class = (def_get_nr_class) lib.GetProcAddress("get_nr_class");
		func_get_labels = (def_get_labels) lib.GetProcAddress("get_labels");

		func_free_model_content = (def_free_model_content) lib.GetProcAddress("free_model_content");
		func_free_and_destroy_model = (def_free_and_destroy_model) lib.GetProcAddress("free_and_destroy_model");
		func_destroy_param = (def_destroy_param) lib.GetProcAddress("destroy_param");

		func_check_parameter = (def_check_parameter) lib.GetProcAddress("check_parameter");
		func_check_probability_model = (def_check_probability_model) lib.GetProcAddress("check_probability_model");
		func_set_print_string_function = (def_set_print_string_function) lib.GetProcAddress("set_print_string_function");

		this->Model = NULL;
	}
	virtual ~XLMachineLearningLibliear()
	{
		for(std::vector<feature_node*>::const_iterator it = ProblemX.begin() ; it != ProblemX.end() ; ++it )
		{
			delete [] *it;
		}
		FreeModel();
	}
	//�t�@�C������w�K�f�[�^��ǂݍ���
	bool LoadTrain(const std::string& filename)
	{
		FILE * fp = fopen(filename.c_str() , "rb");
		if (!fp)
		{
			return false;
		}

		std::vector<char> buffer;
		buffer.resize(65535);
		while (! feof(fp) )
		{
			char * p = &buffer[0];
			fgets(p,buffer.size(), fp);
			if (*p == '#' || *p == '\n' || *p == '\0') continue; //�R�����g�Ƃ�

			char * startN = p;

			//���� : �����邩�𐔂��܂��B
			int featurecount = 0;
			for( ; *startN  ; ++startN ) 
			{
				if (*startN == ':') featurecount ++;
			}

			//�N���X�ԍ��Ƃ̋�؂��
			startN = p;
			for( ; *p != ' ' && *p != '\t' && *p != '\n' && *p != '\0'; ++p );
			if (*p == '\0' || *p == '\n' ) continue;
			*p = '\0';

			//�f���̐������m��(����Ɋm�ۂ��Ȃ��ƃ_��)
			feature_node* feature_nodes = new feature_node[featurecount + 1]; //�I�[������̂� +1����

			//�N���X�ԍ�
			const int classid = atoi(startN);
			ProblemY.push_back(classid);
			ProblemX.push_back(feature_nodes);

			feature_node* featureP = feature_nodes;
			for( ; 1 ; ++featureP )
			{
				//�f���ԍ�
				p ++;
				startN = p;
				for( ; *p != ':' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' ) break;
				*p = '\0';
				featureP->index = atoi(startN);

				//�f���̒l
				p ++;
				startN = p;
				for( ; *p != ' ' && *p != '\t' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' )
				{
					featureP->value = atof(startN);
					break;
				}
				else
				{
					*p = '\0';
					featureP->value = atof(startN);
				}
			}
			//�I�[�}�[�N
			featureP->index = -1;
			featureP->value = 0;
		}
		fclose(fp);
		return true;
	}
	//�������񂾂Ƃ��Ɠ����t�@�C���t�H�[�}�b�g���g���āA�F�����𑪒肵�܂��B �f�o�b�O�p
	bool DebugPredict(const std::string& filename,const std::string& logFilename,int* outall,int* outmatch)
	{
		FILE * fp = fopen(filename.c_str() , "rb");
		if (!fp)
		{
			return false;
		}
		FILE * logfp = fopen(logFilename.c_str() , "wb");
		if (!logfp)
		{
			return false;
		}
		int all = 0;
		int match = 0;

		std::vector<char> buffer;
		buffer.resize(65535);
		while (! feof(fp) )
		{
			char * p = &buffer[0];
			fgets(p,buffer.size(), fp);
			if (*p == '#' || *p == '\n' || *p == '\0') continue; //�R�����g�Ƃ�

			char * startN = p;

			//���� : �����邩�𐔂��܂��B
			int featurecount = 0;
			for( ; *startN  ; ++startN ) 
			{
				if (*startN == ':') featurecount ++;
			}

			//�N���X�ԍ��Ƃ̋�؂��
			startN = p;
			for( ; *p != ' ' && *p != '\t' && *p != '\n' && *p != '\0'; ++p );
			if (*p == '\0' || *p == '\n' ) continue;
			*p = '\0';

			//�ǂ�Ȓl����������ۑ����Ă����܂��B
			std::string feature_string = p + 1;

			//�f���̐������m��(����Ɋm�ۂ��Ȃ��ƃ_��)
			feature_node* feature_nodes = new feature_node[featurecount + 1]; //�I�[������̂� +1����

			//�N���X�ԍ�
			const int classid = atoi(startN);

			feature_node* featureP = feature_nodes;
			for( ; 1 ; ++featureP )
			{
				//�f���ԍ�
				p ++;
				startN = p;
				for( ; *p != ':' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' ) break;
				*p = '\0';
				featureP->index = atoi(startN);

				//�f���̒l
				p ++;
				startN = p;
				for( ; *p != ' ' && *p != '\t' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' )
				{
					featureP->value = atof(startN);
					break;
				}
				else
				{
					*p = '\0';
					featureP->value = atof(startN);
				}
			}
			//�I�[�}�[�N
			featureP->index = -1;
			featureP->value = 0;


			//���ʂ���Ă݂܂��B
			int predictClassID = this->Predict(feature_nodes);

			//���ʂ𑪒肵�܂��B
			if (predictClassID == classid)
			{
				match ++;
				fprintf(logfp,"OK %d %s" , predictClassID , feature_string.c_str() );
			}
			else
			{
				fprintf(logfp,"BAD %d %s" , predictClassID , feature_string.c_str() );
			}
			all ++;

			//�����������Y�ꂸ�ɁB
			delete [] feature_nodes;
		}
		fclose(fp);

		fprintf(logfp,"\r\nAccuracy = %lf%% (%d/%d)" , ((double)match) * 100 / all  , match , all);
		*outall = all;
		*outmatch = match;
		return true;
	}
	//�w�K�f�[�^�̒ǉ� (liblinear ���ނ��I�����C���ł��Ȃ��炵������d���Ȃ����ǂ�)
	void AppendTrain(int classID,const std::vector<feature_node>& newnodes)
	{
		feature_node* nodes = new feature_node[newnodes.size()+1]; //�I�[������̂�+1���Ƃ��܂��B

		ProblemY.push_back(classID);
		ProblemX.push_back(nodes);

		for(std::vector<feature_node>::const_iterator it = newnodes.begin() ; it != newnodes.end() ; ++it )
		{
			*nodes++ = *it;
		}

		//�I�[�}�[�N -1 ������B
		nodes->index = -1;
		nodes->index = 0;
	}
	//�w�K���܂��B
	bool Train()
	{
		int maxindex = 0;
		for(std::vector<feature_node*>::const_iterator it = ProblemX.begin() ; it != ProblemX.end() ; ++it )
		{
			for(feature_node* nodes = *it; nodes->index != -1 ; ++nodes )
			{
				maxindex = max(nodes->index,maxindex);
			}
		}

		struct problem prob;
		prob.bias = -1;				//�o�C�A�X�H����������Ƃ��̈����ŗ^����炵�����E�E�E
		prob.l = ProblemY.size();	//�s��
		prob.n = maxindex;	//��Ԃł����f���ԍ�
		prob.y = &ProblemY[0];
		prob.x = &ProblemX[0];

		struct parameter param;
		param.solver_type = L2R_L2LOSS_SVC_DUAL;
		param.C = 1;
		param.eps = INFINITE; // ���ƂŐݒ肵������
		param.nr_weight = 0;
		param.weight_label = NULL;
		param.weight = NULL;

		if(param.eps == INFINITE)
		{
			if(param.solver_type == L2R_LR || param.solver_type == L2R_L2LOSS_SVC)
				param.eps = 0.01;
			else if(param.solver_type == L2R_L2LOSS_SVC_DUAL || param.solver_type == L2R_L1LOSS_SVC_DUAL || param.solver_type == MCSVM_CS || param.solver_type == L2R_LR_DUAL)
				param.eps = 0.1;
			else if(param.solver_type == L1R_L2LOSS_SVC || param.solver_type == L1R_LR)
				param.eps = 0.01;
		}

		FreeModel();
		this->Model = func_train(&prob,&param);
//		this->Model = train(&prob,&param);
		return this->Model != NULL;
	}
	//�w�K�������f������N���X�ԍ����擾���܂��B
	int Predict(const std::vector<feature_node>& params)
	{
		if (params.size() <= 0)
		{
			return 0;
		}
		if ( (params.rbegin())->index == -1 )
		{
			return func_predict(this->Model , &params[0]);
		}
		else
		{
			std::vector<feature_node> params2 = params;
			feature_node fin;
			fin.index = -1;
			fin.value = 0;
			params2.push_back(fin);

			return func_predict(this->Model , &params2[0]);
		}
	}
	//�w�K�������f������N���X�ԍ����擾���܂��B
	int Predict(const feature_node* params)
	{
		return func_predict(this->Model , params);
	}
	bool SaveModel(const std::string& filename)
	{
		func_save_model(filename.c_str(),this->Model);
//		save_model(filename.c_str(),this->Model);
		return true;
	}
	void FreeModel()
	{
		if (this->Model)
		{
			func_free_model_content(this->Model);
//			free_model_content(this->Model);
			this->Model = NULL;
		}
	}

private:
	std::vector<int> ProblemY;
	std::vector<feature_node*> ProblemX;
	std::vector<feature_node>  RealFeature;
	model* Model;

	LoadLibraryHelper lib;
	typedef struct model* (*def_train)(const struct problem *prob, const struct parameter *param);
	typedef void (*def_cross_validation)(const struct problem *prob, const struct parameter *param, int nr_fold, int *target);

	typedef int (*def_predict_values)(const struct model *model_, const struct feature_node *x, double* dec_values);
	typedef int (*def_predict)(const struct model *model_, const struct feature_node *x);
	typedef int (*def_predict_probability)(const struct model *model_, const struct feature_node *x, double* prob_estimates);

	typedef int (*def_save_model)(const char *model_file_name, const struct model *model_);
	typedef struct model* (*def_load_model)(const char *model_file_name);

	typedef int (*def_get_nr_feature)(const struct model *model_);
	typedef int (*def_get_nr_class)(const struct model *model_);
	typedef void (*def_get_labels)(const struct model *model_, int* label);

	typedef void (*def_free_model_content)(struct model *model_ptr);
	typedef void (*def_free_and_destroy_model)(struct model **model_ptr_ptr);
	typedef void (*def_destroy_param)(struct parameter *param);

	typedef const char* (*def_check_parameter)(const struct problem *prob, const struct parameter *param);
	typedef int (*def_check_probability_model)(const struct model *model);
	typedef void (*def_set_print_string_function)(void (*print_func) (const char*));


	def_train func_train;
	def_cross_validation func_cross_validation;

	def_predict_values func_predict_values;
	def_predict func_predict;
	def_predict_probability func_predict_probability;

	def_save_model func_save_model;
	def_load_model func_load_model;

	def_get_nr_feature func_get_nr_feature;
	def_get_nr_class func_get_nr_class;
	def_get_labels func_get_labels;

	def_free_model_content func_free_model_content;
	def_free_and_destroy_model func_free_and_destroy_model;
	def_destroy_param func_destroy_param;

	def_check_parameter func_check_parameter;
	def_check_probability_model func_check_probability_model;
	def_set_print_string_function func_set_print_string_function;

};


int main()
{
	XLMachineLearningLibliear liblinear;
	{
		g_TrainFile = new std::ofstream("train.txt");

		if ( ! JuliusFileStart() )
		{
			return 1;
		}

		//���s�����f�[�^�̊w�K.
		g_OKorBAD = false;
		ScanDir("bad_wav");

		//���������f�[�^�̊w�K.
		g_OKorBAD = true;
		ScanDir("ok_wav");

		delete g_TrainFile;
	}
	{
		g_TrainFile = new std::ofstream("test.txt");

		if ( ! JuliusFileStart() )
		{
			return 1;
		}

		//���s�����f�[�^�̊w�K.
		g_OKorBAD = false;
		ScanDir("test_bad_wav");

		//���������f�[�^�̊w�K.
		g_OKorBAD = true;
		ScanDir("test_ok_wav");

		delete g_TrainFile;
	}
	//�t�@�C������w�K�f�[�^��ǂݍ���
	liblinear.LoadTrain("train.txt");
	//�w�K����
	liblinear.Train();

	//�e�X�g����
	int all,match;
	liblinear.DebugPredict("train.txt" , "log_train.txt" , &all,&match);
	printf("�w�K�����f�[�^���ăe�X�g: Accuracy = %.3lf%% (%d/%d)   �ڍׂ�log_train.txt\r\n" , ((double)match) * 100 / all  , match , all);
	liblinear.DebugPredict("test.txt" , "log_test.txt" , &all,&match);
	printf("���m�̃f�[�^�ł̃e�X�g  : Accuracy = %.3lf%% (%d/%d)   �ڍׂ�log_test.txt \r\n" , ((double)match) * 100 / all  , match , all);


	//�w�K���f����ۑ�����
	liblinear.SaveModel("train.dat");
#ifdef _MSC_VER
	printf("�G���^�[�L�[�ŏI�����܂��B");
	//�L�[���͑҂�
	std::string line;
	getline( std::cin, line );
#endif
	return 0;
}