// XLUrlParser.cpp: XLUrlParser �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "XLUrlParser.h"

USE_TEST(XLUrlParser)


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XLUrlParser::XLUrlParser()
{

}

XLUrlParser::~XLUrlParser()
{

}

void XLUrlParser::Parse(const string & inURL)
{
	this->Protocol = "http";
	this->UserName = "";
	this->Password = "";
	this->Host = "";
	this->Port = -1;
	this->Path = "/";

	const char * url = inURL.c_str();

	const char * slash = strstr(url , "/");

	//�v���g�R��
	const char * slashslash = strstr(url , "://");
	if (slashslash != NULL)
	{
		this->Protocol = string(url ,  0 , slashslash - url );
		url = slashslash + 3;
	}

	//���[�U���A�p�X���[�h�A�z�X�g���ƃ|�[�g�ԍ��̕��������𔲂��o��
	string host;
	const char * slash2 = strstr(url , "/");
	if (slash2 == NULL)
	{
		host = url;
	}
	else
	{
		host = string(url , 0 , slash2 - url );
		this->Path = slash2;
	}

	//���[�U���ƃp�X���[�h
	const char * user = strstr(host.c_str() , "@");
	if (user != NULL)
	{
		this->UserName = string(host.c_str() , user - host.c_str() );

		const char * password = strstr(user + 1 , ":");
		if (password != NULL)
		{
			this->UserName = string(user + 1, password - (user + 1) );
		}
		host = password + 1;
	}

	//�z�X�g���ƃ|�[�g�ԍ�
	const char * port = strstr(host.c_str() , ":");
	if (port != NULL)
	{
		this->Port = atoi( port + 1 );
		this->Host = string( host.c_str() , port - host.c_str() );
	}
	else
	{
		this->Host = host;
	}

	//�|�[�g�̎����F��
	if (this->Port == -1)
	{
		if (this->Protocol == "http")
		{
			this->Port = 80;
		}
		else if (this->Protocol == "https")
		{
			this->Port = 443;
		}
	}
}

void XLUrlParser::test()
{
	{
		XLUrlParser a;
		string r;
		int rr;
		a.Parse("http://www.yahoo.co.jp/");
		ASSERT( (r = a.getProtocol() ) == "http");
		ASSERT( (r = a.getUserName() ) == "");
		ASSERT( (r = a.getPassword() ) == "");
		ASSERT( (rr = a.getPort() ) == 80);
		ASSERT( (r = a.getHost() ) == "www.yahoo.co.jp");
		ASSERT( (r = a.getPath() ) == "/");
	}
	{
		XLUrlParser a;
		string r;
		int rr;
		a.Parse("http://www.yahoo.co.jp:1192/");
		ASSERT( (r = a.getProtocol() ) == "http");
		ASSERT( (r = a.getUserName() ) == "");
		ASSERT( (r = a.getPassword() ) == "");
		ASSERT( (rr = a.getPort() ) == 1192);
		ASSERT( (r = a.getHost() ) == "www.yahoo.co.jp");
		ASSERT( (r = a.getPath() ) == "/");
	}
	{
		XLUrlParser a;
		string r;
		int rr;
		a.Parse("www.yahoo.co.jp:1192/");
		ASSERT( (r = a.getProtocol() ) == "http");
		ASSERT( (r = a.getUserName() ) == "");
		ASSERT( (r = a.getPassword() ) == "");
		ASSERT( (rr = a.getPort() ) == 1192);
		ASSERT( (r = a.getHost() ) == "www.yahoo.co.jp");
		ASSERT( (r = a.getPath() ) == "/");
	}
	{
		XLUrlParser a;
		string r;
		int rr;
		a.Parse("www.yahoo.co.jp/");
		ASSERT( (r = a.getProtocol() ) == "http");
		ASSERT( (r = a.getUserName() ) == "");
		ASSERT( (r = a.getPassword() ) == "");
		ASSERT( (rr = a.getPort() ) == 80);
		ASSERT( (r = a.getHost() ) == "www.yahoo.co.jp");
		ASSERT( (r = a.getPath() ) == "/");
	}
	{
		XLUrlParser a;
		string r;
		int rr;
		a.Parse("www.yahoo.co.jp/abcdf/ewfew/a.php");
		ASSERT( (r = a.getProtocol() ) == "http");
		ASSERT( (r = a.getUserName() ) == "");
		ASSERT( (r = a.getPassword() ) == "");
		ASSERT( (rr = a.getPort() ) == 80);
		ASSERT( (r = a.getHost() ) == "www.yahoo.co.jp");
		ASSERT( (r = a.getPath() ) == "/abcdf/ewfew/a.php");
	}
}