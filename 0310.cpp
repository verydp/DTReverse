#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable : 4996)

/*
int fseek(FILE *stream, long int offset, int whence)
	stream -- ����ָ�� FILE �����ָ�룬�� FILE �����ʶ������
	offset -- ������� whence ��ƫ���������ֽ�Ϊ��λ��
	whence -- ���Ǳ�ʾ��ʼ���ƫ�� offset ��λ�á���һ��ָ��Ϊ���г���֮һ��
		SEEK_SET	�ļ��Ŀ�ͷ
		SEEK_CUR	�ļ�ָ��ĵ�ǰλ��
		SEEK_END	�ļ���ĩβ
*/

int main() {

	//����һ���ļ�����
	FILE* pfile;
	FILE* retfile;
	//����һ����������ļ���С�ı���
	long lSize;
	//����һ��ָ�룬����ָ���ڴ��д���ļ����ڴ���ʼ��ַ
	char* buffer;
	//����һ�������������ж϶�ȡ���ڴ�Ŀռ��С���Ƿ���ļ������Сһ�¡�
	size_t result;

	//�Զ����Ƶķ�ʽ��һ���ļ���
	pfile = fopen("C:\\Program Files\\WindowsApps\\Microsoft.WindowsNotepad_11.2503.16.0_x64__8wekyb3d8bbwe\\Notepad\\Notepad.exe", "rb");

	//ͨ��fseek()���ļ�ָ��λ�����õ�END��ͨ��ftell()��ȡ���ļ��Ĵ�С
	fseek(pfile,0,SEEK_END);
	lSize = ftell(pfile);

	printf("%d\n", lSize);

	//C �⺯�� void rewind(FILE * stream) �����ļ�λ��Ϊ������ pfile �ļ��Ŀ�ͷ��
	rewind(pfile);

	//�����ϵͳ����һ���ڴ�ռ䡣
	buffer = (char*)malloc(sizeof(char)*lSize);

	//ͨ��fread()���������ļ�������buffer�С�
	if (buffer != NULL) {
		fread(buffer, 1, lSize, pfile);
	}
	else {
		printf("Can\'t copy the file to buffer~");
	}
	//�ӳ����е�ĳ���ڴ��������д��ĳ���ļ��С�
	retfile = fopen("C:\\Users\\84519\\Desktop\\notepad.exe", "w+");
	if (buffer != NULL) {
		fwrite(buffer, 1, lSize, retfile);
	}
	else {
		printf("Can\'t get buffer~");
	}
	

	fclose(pfile);
	free(buffer);
	return 0;
}


