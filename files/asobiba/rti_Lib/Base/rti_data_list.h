
class TCommandList
{
    char **CL;

    public:
        //�R�}���h���X�g�ݒ�.
        void set(char **C){CL = C;};
        //�T�[�`
        int search(char *str,char **NextChar);
};
