#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int isDirectory(char* path)
{
	DIR* temp;
	if ((temp = opendir(path)) == NULL)
		return 0;
	closedir(temp);
	return 1;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("usage : %s <directory_path>\n", argv[0]);
		return -1;
	}

	DIR* dirp;
	struct dirent* dentry;
	int nFiles = 0;
	int nDirectories = 0;

	if ((dirp = opendir(argv[1])) == NULL)
	{
		printf("There is no directory <%s>\n", argv[1]);
		return -1;
	}
	while (dentry = readdir(dirp))
	{
		if (dentry->d_ino)
		{
			if (isDirectory(dentry->d_name))
			{
				nDirectories++;
			}
			else
			{
				nFiles++;
			}
		}
		//printf("%s\n", dentry->d_name);
	}
	printf("number of files:\t%d\n", nFiles);
	printf("number of directories:\t%d\n", nDirectories);
	closedir(dirp);
}
