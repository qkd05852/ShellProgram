#include "myHeader.h"


#define MAX 128	//buffer maximum size

void prompt(char []); // 프롬프트 출력과 명령어 입력
void stdout_redirection_execute(char *[], char *[]); // '>' 리다이렉션
void stdin_redirection_execute(char *[], char *[]); // '<' 리다이렉션
void background_process_sigfunc(int signo); // 백그라운드 처리

// 명령어 함수
void change_directory(char *[]);
void mypwd(char *[]);
void myls(char *[]);
void mycmp(char *[]);
void mycp(char *[]);
void mymv(char *[]);
void mymkdir(char *[]);
void myrmdir(char *[]);
void mycat(char *[]);
void myhead(char *[]);
void mytail(char *[]);
void myrm(char *[]);

int main(void)
{
	char buffer[MAX] = {0,}; // 명령어 입력을 위한 버퍼
	char buffer2[MAX] = {0,}; // 추가적인 파싱을 위한 버퍼
	char *commandSet[10]; // 파싱된 명령어 저장을 위한 포인트배열
	char *rear_command[10]; // 파이프,리다이렉션을 위한 포인트배열
	int flag = 0; // '>', '<', '|' 구분을 위한 플래그

	while(1)
	{
		memset(commandSet, 0x00, sizeof(char *)*10);  
		memset(rear_command, 0x00, sizeof(char *)*10);
		memset(buffer, 0x00, sizeof(buffer));
		memset(buffer2, 0x00, sizeof(buffer2));	
		flag = 0;

		prompt(buffer); // 프롬프트 출력

		if(strlen(buffer) == 0) // if you enter anything, continue the procedure
			continue;
		if(strcmp(buffer, "exit") == 0) // 종료
		{
			fprintf(stdout, "Goodbye! Shellprogram is finished\n");
			return 0;
		}
	}
	return 0;
}

void prompt(char buffer[]) 
{
	char cur_working_dir[MAX] = {0,};//현재 디렉터리의 경로를 저장하기 위한 배열
	
	getcwd(cur_working_dir, MAX); //현재 디렉터리
	
	printf("[%s %s] $ ", "eight team Shellprogram", strrchr(cur_working_dir, '/')+1); // 프롬프트 출력
	

	fgets(buffer, MAX, stdin); // 표준입력으로부터 명령어를 받아 버퍼에 저장
	buffer[strlen(buffer)-1] = '\0';
	fflush(stdin);
}


void stdin_redirection_execute(char *commandSet[], char *rear_command[])
{
	int fd, in; // fd = 파일 저장을 위한 파일 디스크립터 변수                             in = 표준입력을 복사, 저장하기위한 변수
	
	in = dup(0); // dep(0)을 통해 in변수에 표준입력을 복사 저장
	if((fd = open(rear_command[0], O_RDONLY, 0644)) < 0)
	{
		fprintf(stderr, "ERROR : File open error ocurred\n");
		exit(1);
	}
	fflush(stdin);
	close(0);
	dup2(fd, 0); // file fd => input
	close(fd);
	execute(commandSet, 0);
	close(0);
	dup2(in, 0); // change to stdin
}
void background_process_sigfunc(int signo)
{
	pid_t pid; // 자식 프로세스의 pid를 저장하기 위한 변수
	int status; // 자식 프로세스의 상태를 저장하기 위한 변수

	while((pid = waitpid(-1, &status, WNOHANG)) > 0) // signal을 받아 이 함수를 실행시키고 함수가 실행되면 백그라운드 프로세스의 pid를 출력하면서 자식프로세스를 종료. 백그라운드 프로세스가 없을 때 까지 반복문이 실행

	{
		printf("Background process finished : %d\n", pid);
	}
}
