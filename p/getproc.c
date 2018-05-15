#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

struct procstat
{
	pid_t pid;
	char comm[NAME_MAX];
};
int main(void)
{

}

int filter(const struct dirent *entry)
{

}

int getproc(const char *procname, struct procstat *procv)
{
	
}
