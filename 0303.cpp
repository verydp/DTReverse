#include <cstdio>
#include <string.h>


//1������һ��int* arr[5] ���飬��Ϊ���鸳ֵ(ʹ��&).

int* arr[5];

void function3_3_1() {
	arr[1] = (int*)2;

	int** x = &arr[0];
	*x = (int*)11;

}

//2������һ���ַ�ָ�����飬�洢���е�C�Ĺؼ���(��������)����ȫ����ӡ����.

char* key[32];

void function3_3_2() {
	/*
	�������͹ؼ��֣�12����
	void�����������޷���ֵ���޲���������������ָ��23��
	char�������ַ��ͱ�������34��
	int���������ͱ�������34��
	float�����������ȸ����ͱ�������3��
	double������˫���ȸ����ͱ�������34��
	enum������ö������4��
	short�����������ͱ�������34��
	long�����������ͱ�������34��
	signed�������з������ͱ�������34��
	struct�������ṹ���������34��
	union��������������������34��
	unsigned�������޷������ͱ�������34��
	*/
	key[0] = (char*)"void";
	key[1] = (char*)"char";
	key[2] = (char*)"int";
	key[3] = (char*)"float";
	key[4] = (char*)"double";
	key[5] = (char*)"enum";
	key[6] = (char*)"short";
	key[7] = (char*)"long";
	key[8] = (char*)"signed";
	key[9] = (char*)"struct";
	key[10] = (char*)"union";
	key[11] = (char*)"unsigned";
	/*
	�������ؼ��֣�12����
	if��������䡣
	else���������ķ񶨷�֧���� if ���á�
	switch��������䡣
	case���������ķ�֧��ǩ��
	default����������еġ���������֧��
	for��ѭ����䡣
	do��ѭ������ѭ����4��
	while��ѭ������ѭ��������
	break��������ǰѭ���� switch �ṹ��
	continue��������ǰѭ����ʣ�ಿ�֣���ʼ��һ��ѭ����
	goto����������ת��䡣
	return���ӳ��򷵻���䡣
	*/
	key[12] = (char*)"if";
	key[13] = (char*)"else";
	key[14] = (char*)"switch";
	key[15] = (char*)"case";
	key[16] = (char*)"default";
	key[17] = (char*)"for";
	key[18] = (char*)"do";
	key[19] = (char*)"while";
	key[20] = (char*)"break";
	key[21] = (char*)"continue";
	key[22] = (char*)"goto";
	key[23] = (char*)"return";
	/*
	�洢���͹ؼ��֣�4����
	auto�������Զ�������
	extern�������������������ļ���������
	register�������Ĵ���������
	static��������̬������
	*/
	key[24] = (char*)"auto";
	key[25] = (char*)"extern";
	key[26] = (char*)"register";
	key[27] = (char*)"static";
	/*
	�����ؼ��֣�4����
	const������ֻ��������
	sizeof�������������ͻ�������ڴ�����ռ���ֽ�����
	typedef�����Ը���������ȡ������
	volatile��˵�������ڳ���ִ���пɱ������ظı䡣
	*/
	key[28] = (char*)"const";
	key[29] = (char*)"sizeof";
	key[30] = (char*)"typedef";
	key[31] = (char*)"volatile";

	for (int x = 0;x < 32;x++) {
		for (int i = 0;*(key[x] + i) != 0;i++) {
			printf("%c", *(key[x] + i));
		}
		printf("\n");
	}
	
	//printf("%c\n", *(key[2]+2));
}


//3��������Щ�����У��м���id=1 level=8�Ľṹ����Ϣ��
char array[100] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,
	0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,
	0x00,0x33,0x00,0x47,0x0C,0x0E,0x00,0x0D,0x00,0x11,
	0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0xAA,0x00,
	0x00,0x00,0x64,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,
	0x00,0x02,0x57,0x4F,0x57,0x00,0x06,0x08,0x00,0x00,
	0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,
	0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00
};

typedef struct TagPlayer {
	int id;
	int level;
}Player;

int function3_3_3() {
	Player* p;
	//p = (Player*)array;

	for (int i = 0;i < 100;i++) {
		p = (Player*)(array + i);
		if (p->id == 1 && p->level == 8) {
			printf("%d\n", i);
		}
		printf("%x\n", p->id);

	}

	return 0;
}

int main() {

	function3_3_3();

}