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


long sys_getcwd(char * buf, size_t size)
{
	struct m_inode *inode,*inode_f;
	char result[128],base[128];
	struct buffer_head * bh;
	struct dir_entry *dir,*tmp;
	unsigned short nownode,rootnode;
    int nowblock,nowi_dev;
	int entries,i;

	result[0]=0;//初始
    inode=current->pwd, inode_f=current->root;
    nowblock=inode->i_zone[0];	  
	nowi_dev=inode->i_dev;
	if(!(nowblock = inode->i_zone[0]) || !(bh = bread(inode->i_dev,nowblock)) || !nowi_dev)
		return NULL;		


	
	dir = (struct dir_entry *) bh->b_data;
	nownode=dir->inode;
	dir++;
	rootnode=dir->inode;


	while (inode!=inode_f)
	{
		//update
		inode=iget(nowi_dev,rootnode); 
		if (!(nowblock = inode->i_zone[0]))
			return 0;
		nowi_dev=inode->i_dev;		 
		if(!nowi_dev)
			return 0;
		if (!(bh = bread(inode->i_dev,nowblock)))
			return 0;
		dir = (struct dir_entry *) bh->b_data;
		tmp=dir;

		entries = inode->i_size / (sizeof (struct dir_entry));

		for(i=0;i<entries;++i)
		{
			if(tmp->inode==nownode)
			{
				base[0]=0;
				strcat(base,"/");
				strcat(base,tmp->name);
				strcat(base,result);
				strcpy(result,base);
				break;
			}
			++tmp;
		}

		
		nownode=rootnode;
		rootnode=(++dir)->inode;
		
	}

	size_t len=strlen(result);
	if(len==0)
	{
		strcpy(result,"/");
		len++;
	}
	if(size<len)
	{
		return 0;
	}
	char *k=buf;
	for(i=0;i<len;++i)
		put_fs_byte(result[i],k++);
	return buf;
}








