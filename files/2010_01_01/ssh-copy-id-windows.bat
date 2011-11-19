@echo off
echo ssh�L�[�ݒ�v���O���� by rti

:step0
rem "OpenSSH for windows���C���X�g�[������Ă��邩�`�F�b�N"
ssh -v 2> NUL
if not %ERRORLEVEL% == 1 goto ssh_not_found

:step1
rem ".ssh�f�B���N�g�������邩�ǂ����`�F�b�N�B�Ȃ��Ȃ�쐬����"
if NOT EXIST "%USERPROFILE%\.ssh" goto make_ssh_directory

:step2
rem "rsa�L�[���L�邩�ǂ����`�F�b�N�B�Ȃ��Ȃ�쐬����"
if NOT EXIST "%USERPROFILE%\.ssh\id_rsa.pub" goto make_ssh_keygen

:step3
rem "����]������T�[�o���ƃ��[�U���𕷂�"
echo.
echo 1.ssh�L�[��o�^����T�[�o��/IP�A�h���X����͂��Ă��������B
set /p SSH_SERVERNAME="�T�[�o��>"
echo 2.���[�U������͂��Ă��������B ��������Ȃ��ƌ��݂̃��[�U�� %USERNAME% �ɂȂ�܂��B
set /p SSH_USERNAME="���[�U��>"
if "%SSH_USERNAME%" == "" set SSH_USERNAME="%USERNAME%"

:step4
rem "���Ɍ���]�����Ă��Ȃ����m�F����"
echo.
echo �Ȃɂ��L�[�������ƁA%SSH_USERNAME%@%SSH_SERVERNAME% �T�[�o��ssh�Ń��O�C�����L�[��o�^���܂��B
pause
ssh  -o PreferredAuthentications=publickey %SSH_USERNAME%@%SSH_SERVERNAME% "exit 72"
if %ERRORLEVEL% == 72 goto ssh_already_keyauth

:step5
rem "����]������"
echo.
type "%USERPROFILE%\.ssh\id_rsa.pub" | ssh %SSH_USERNAME%@%SSH_SERVERNAME% "umask 077; test -d .ssh || mkdir .ssh ; cat >> .ssh/authorized_keys ; exit 72"
if not %ERRORLEVEL% == 72 goto ssh_keycopy_error

:step6
rem "�]���łł������m�F����"
echo.
echo �����ɃL�[�F�؂Ń��O�C�����Ă݂܂��B
ssh  -vvv -o PreferredAuthentications=publickey %SSH_USERNAME%@%SSH_SERVERNAME% "exit 72" 2> "%TEMP%\\ssh-copy-id-rti-temp.txt"
if not %ERRORLEVEL% == 72 goto ssh_loginerror
del "%TEMP%\\ssh-copy-id-rti-temp.txt"

:step7
rem "�����܂�"
echo.
echo OK.�L�[�F�؂��������܂����B
echo �����L�[�������ƏI�����܂��B
pause
exit 1

:ssh_already_keyauth
echo.
echo ����H�������łɃL�[�F�؂ł��܂���B
echo �����L�[�������ƏI�����܂��B
pause
exit 1

:ssh_loginerror
echo.
echo �L�[�F�؂Ń��O�C���ł��܂���ł����B
echo �T�[�o�̐ݒ肪���������̂��ȁH
echo �ȉ��ڍׂȃG���[�t���[
type "%TEMP%\\ssh-copy-id-rti-temp.txt"
del "%TEMP%\\ssh-copy-id-rti-temp.txt"
echo �Ȃɂ��L�[�������ƁA�L�[�R�s�[�����蒼���܂�
pause
goto step3


:ssh_keycopy_error
echo.
echo ssh�L�[���R�s�[�ł��܂���ł����B�B�B
echo �T�[�o���⃆�[�U�[���͊Ԉ���Ă��܂��񂩁H
echo �Ȃɂ��L�[�������Ƃ�����x���܂�
pause
goto step3

:make_ssh_directory
echo.
mkdir "%USERPROFILE%\.ssh"
goto step1

:make_ssh_keygen
echo.
echo ssh�L�[(rsa)��������܂���B
echo �����L�[�������ƁAssh-keygen���N������ssh�L�[���쐬���܂�
pause
cd %USERPROFILE%\.ssh
echo �m���p�X�ł�肽���l��enter�L�[��3��A�ł��Ă�������www
echo.
ssh-keygen -t rsa
goto step2

:ssh_not_found
echo.
echo ssh not found.
echo OpenSSH���C���X�g�[�����ĉ�����
pause
exit 1

