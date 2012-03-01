/*
c-like lua 
*/
#include <windows.h>
#include <assert.h>
#include "lua/lua.hpp"


//lua�ɓo�^����֐�
int l_cecho(lua_State* L)
{
	//��1����
	if (! lua_isstring(L,-1) )
	{
		return luaL_error(L,"���������Ȃ���");
	}
	printf("c_echo ������: %s\r\n" , lua_tostring(L,-1) );

	//�߂�l
	lua_pushstring(L,"'���\�[�̕\'�߂�l���ɂ�[");

	//lua_setusertag �œ��ꂽ�l������悤�ɂ��Ă݂��B
	//this�̈����񂵂Ƃ����邢����A����ɂ��ȃC�J�H
	//
	//�ӂ[�ɏ����ƁA�Z�b�g����
	//lua_pushlightuserdata(L ,(void*) this);
	//lua_setfield(L,"_this");
	//
	//���o���A�߂�ǂ�
	//lua_getfield(L,LUA_REGISTRYINDEX,"_this");
	//myclass* p = (myclass*)lua_touserdata(L,-1);
	//lua_pop(L,1);
	//
	assert( (unsigned long)lua_getusertag(L) == (unsigned long)0xff00ff00);


	return 1;             //�߂�l�̐����w�� 1��
}

int main()
{
    // Lua���J��
    lua_State* L = luaL_newstate();
	//�}���`�o�C�g(SJIS�Ƃ�)���g����悤�ɏC������t���O������Ă݂��B
	lua_setmultibyte(L,1);
	//���ł� this �����񂵂Ɏg����t���O��ǉ����Ă݂��B
	lua_setusertag(L,(void*)0xff00ff00);


	//������̊֐��o�^.
	lua_register(L, "l_cecho", (lua_CFunction)l_cecho);

    // Lua�W���֐����g��
    luaL_openlibs(L);

    // Lua�t�@�C����ǂݍ���
	
    if(luaL_loadfile(L, "sample.lua") || lua_pcall(L, 0, 0, 0)) {
        perror(lua_tostring(L, -1));
    }

	//add�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "add");
		//��1����x
		lua_pushnumber(L, 10);
		//��2����y
		lua_pushstring(L,"20");

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 2, 1, 0) != 0) {
				printf("add(10,20)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("add(10,20)���� : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	//factorial_while�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "factorial_while");
		//��1����n
		lua_pushnumber(L, 10);

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("factorial_while(10)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("factorial_while(10)���� : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	//factorial_for�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "factorial_for");
		//��1����n
		lua_pushnumber(L, 10);

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("factorial_for(10)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("factorial_for(10)���� : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	//yamaorkawa�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "yamaorkawa");
		//��1����n
		lua_pushstring(L, "yama");

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa(yama)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa(yama)���� : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	//yamaorkawa_luastyle�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "yamaorkawa_luastyle");
		//��1����n
		lua_pushstring(L, "yama");

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa_luastyle(yama)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa_luastyle(yama)���� : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	//yamaorkawa_sjis�֐��f��
	{
		//add�֐����X�^�b�N�ɐς�
		lua_getglobal(L, "yamaorkawa_sjis");
		//��1����n
		lua_pushstring(L, "yama");

		//add(x, y)���Ăяo�� ����2�C�߂�l1��
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa_sjis(yama)�֐��Ăяo�����s\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa_sjis(yama)���� : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //�߂�l���|�b�v
		}
	}
	

    // Lua�����
    lua_close(L);

    printf("�G���^�[�L�[�������ƏI�����܂����\n");
	getchar();
    return 0;
}