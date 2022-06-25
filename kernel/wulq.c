#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#include <sys/times.h>
#include <sys/utsname.h>


int sys_pipe2(void){
	return 0;
}
int sys_execve2(const char * path, char * argv[], char * envp[]){
	printk("hello,welcome\n");
	return 0;
}
int sys_getdents(unsigned int fd, struct linux_dirent *drip, unsigned int count ){
	printk("hello,welcome\n");
	struct file * file;
	struct m_inode * inode ;
	struct buffer_head * bh;
	struct linux_dirent *tmp;
	char * wulq;
	struct dir_entry *dir;
	int cnt = 0;

	tmp = (struct linux_dirent *) malloc(count + 64);
	file = current->filp[fd];
	if (fd >= NR_OPEN || !(file = current->filp[fd]) ||
	    !(inode= file->f_inode)||!count)
		return -1;
	bh = bread(inode->i_dev,inode->i_zone[0]);

	int size = sizeof(long)+sizeof(off_t)+sizeof(unsigned short);
	int i;
	int l_dir = sizeof(struct dir_entry);
	int l_dirent = size + sizeof(dir->name);
	int dir_read ;
	dir_read = 0;

	while(cnt<=count-l_dirent && dir_read<=inode->i_size){
		dir = (struct dir_entry *)(bh->b_data + dir_read);
		if(!dir->inode){
			dir_read += l_dir;
			continue;
		}
		tmp->d_ino  = dir->inode;
		tmp->d_off = 0;
		tmp->d_reclen = l_dirent;
		strcpy(tmp->d_name,dir->name);
		for(i = 0;i< l_dirent;i++){
			put_fs_byte(*((char*)tmp+i),((char*)drip)+i+cnt);
		}
		cnt += l_dirent;
		dir_read += l_dir;
	}
	return cnt;
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







