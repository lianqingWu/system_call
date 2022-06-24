#include <unistd.h>
//#include <linux/sys.h>
#include <errno.h>
#include <string.h>
#include<asm/segment.h>
#include <linux/kernel.h>
#include <signal.h>
#include <linux/fs.h>


int sys_pipe2(void){
	return 0;
}
int sys_execve2(const char * path, char * argv[], char * envp[]){
	printk("hello,welcome\n");
	return 0;
}
int sys_getdents(unsigned int fd, struct linux_dirent *drip, unsigned int count ){
	printk("hello,welcome\n");
	int error;
	struct file * file;
	struct inode * inode;

	if (fd >= NR_OPEN || !(file = current->filp[fd]) ||
	    !(inode = file->f_inode))
		return -EBADF;
	error = -ENOTDIR;
	if (file->f_op && file->f_op->readdir) {
		error = verify_area(VERIFY_WRITE, dirent, sizeof (*dirent));
		if (!error)
			error = file->f_op->readdir(inode,file,dirent,count);
	}
	return error;



	return 0;
}


void handler(int signo)
{
}

int  sys_sleep(unsigned int seconds){
	printk("hello,welcome\n");
	struct	sigaction act,oact;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sys_signal(SIGALRM,SIG_IGN,NULL);

	sys_sigaction(SIGALRM,&act,&oact);
	sys_alarm(seconds);
	pause();
	int ret = sys_alarm(0);
	sys_sigaction(SIGALRM,&oact,NULL);
	
	return ret;
}
long sys_getcwd(char * buf, size_t size){
	printk("hello,welcome\n");


	return 0;
}







