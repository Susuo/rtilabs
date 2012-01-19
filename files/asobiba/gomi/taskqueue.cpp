/*
taskqueue
��肽������:�^�X�N�L���[����肽���B
�^�X�N�X���b�h�������Ă���̂ŁA�����Ƀ^�X�N�� lambda(�֐��I�u�W�F�N�g)�ŗ^����ƁA�ǂ�ǂ���s���Ă����݂����ȁB

����ė~�����^�X�N�����b�Z�[�W�ł͂Ȃ��āAlambda �ŗ^����̂������H
C#�� Invoke() �݂����ȃm���œ����^�X�N�L���[���ق����B


�ŁA�Ƃ肠������������ǁA
�Ȃ����A�^�X�N���L���[�ɐς�ł����ԂŁA�X���b�h���~�߂悤�Ƃ���ƒ�~�t���O�𖳎����ăt���[�Y������ۂ��B
���ł��낤�H
boost::unique_future�搶���ςȂ��Ƃ��Ă�̂��ȁH

*/
#include <iostream>
#include <boost/thread.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/move/move.hpp>
#include <boost/bind.hpp>
#include <deque>
//#include <functional>

//http://stackoverflow.com/questions/5889532/how-do-i-create-a-queue-that-holds-boostpackaged-task-with-functions-that-re
template<typename Functor> class taskqueue
{
public:
	typedef typename boost::result_of<Functor()>::type result_type;

//	typedef int result_type;
public:
	//�L���[�Ƀ^�X�N��ς�
	boost::unique_future<result_type> push_async(Functor functor)
	{
		boost::packaged_task<result_type> task(functor);
		boost::unique_future<result_type> future = task.get_future();
		{
			boost::unique_lock<boost::mutex> al(this->lock);
			internal_queue.push_back(boost::move(task));
//���ƃR���p�C���G���[�ɁE�E�E �܂��͂��Ȃ��ł����߂�.
//			//boost::shared_ptr is possibly move-enabled so you can try moving it
//			internal_queue.push_back( boost::bind(dereference_functor(), task) );
		}
		return boost::move(future);
	}
	//�L���[�Ƀ^�X�N��ς�ő����s SendMessage�݂����Ȃ̂��
	result_type push_sync(Functor functor)
	{
		boost::unique_future<ret_type> r = push_future(Functor functor);
		return r.get();
	}

	//�X���b�h���Ăяo���B
	//�^�X�N�L���[�����āA�^�X�N������Ύ��s����B
	//�Ȃ���� false ��Ԃ��ĉ������Ȃ�.
	bool execute()
	{
		boost::packaged_task<result_type> task;
		{
			boost::unique_lock<boost::mutex> al(this->lock);
			if (internal_queue.empty() )
			{
				return false;
			}
			task = boost::move( internal_queue.front() );
			internal_queue.pop_front();
		}
		task();
		return true;
	}

	std::deque< boost::packaged_task<result_type> > internal_queue;
	boost::mutex lock;
	boost::condition_variable queue_wait;
};

class threadFunc
{
public:
//	int operator()()
	int main()
	{
		boost::this_thread::interruption_enabled();
		while(1)
		{
			boost::this_thread::interruption_point();
			queue.execute();
		}
		return 0;
	}

	void append_task(int arg1)
	{
		queue.push_async(
			[arg1]() -> int {
				std::cout << "Task �X�^�[�g! Number:" << arg1 << std::endl;
				boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(2000));

				std::cout << "Task �I��! Number:" << arg1 << std::endl;
				return 0; 
			}
		);
	}

	taskqueue< boost::function<int (void)> > queue;
};

int main() 
{
	threadFunc obj;
	boost::thread th(boost::bind(&threadFunc::main, &obj));

	int task_number = 0;

	while(1)
	{
		std::cout << "�R�}���h������ �G���^�[�L�[�����Ă��������B(q:�I�� / a:�^�X�N�̒ǉ�) " << std::endl;
		std::string str;
		getline( std::cin, str );
		if (str == "q")
		{
			break;
		}
		if (str == "a")
		{
			obj.append_task(task_number);
			std::cout << "task " << task_number << " ��o�^���܂����B" << std::endl;
			task_number++;
		}

	}
	std::cout << " ( ߄t�)�߶��" << std::endl;
	std::cout << "interrupt ��~���ߑ��M" << std::endl;
	th.interrupt();
	std::cout << "join ��~�҂�" << std::endl;
	th.join();
	std::cout << "�v���O�������I�����܂�" << std::endl;
	return 0;
}
