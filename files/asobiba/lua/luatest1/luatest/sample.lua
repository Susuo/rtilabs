-- c-like lua
function add(x,y) {
    return x + y
}

//�K��
function factorial_while(n) {
    //while�ŊK�悵�Ă݂�
    sum = 1
    i = 1
    while(i <= n){
       sum = sum * i
       i = i + 1
    }
    return sum
}

//�K��
function factorial_for(n) {
    //for�ŊK�悵�Ă݂�
    //���܂Ȃ����A lua ��for�� ���S��c-like�ɂ�����Ă��Ȃ���
    sum = 1
    for (i = 1 , n , 1 ) { //or for (i = 1 , n ) {
       sum = sum * i;
    }
    return sum
}

//�R��
function yamaorkawa(str) {
    table = {} //���������v
    if (str == "yama") {
         return "kawa"
    }
    else if (str == "kawa") {
         return "yama"
    }
    else {
         return "nazo";
    }
}

//�R��
function yamaorkawa_luastyle(str)
    if str == "yama" then
         return "kawa"
    elseif str == "kawa" then
         return "yama"
    else
         return "nazo";
    end
end

//�R��
function yamaorkawa_sjis(str) {
    if (str == "�R") {
         return "��"
    }
    else if (str == "��") {
         return "�R"
    }
    return "�C�~�twww"
}

//�_��������ŏo��
print( l_cecho("���\�j�[�̕\") );
