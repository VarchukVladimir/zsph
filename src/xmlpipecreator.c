#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "../config/config.h"


#define myout stdout
#define fout(st) fprintf (myout, "%s", st);
#define USE_XMLPIPE2 0

void getworsfromfile ( char * );
char * getext (char * );
void myxmlpipe (char *);
void myxmlpipe2 (char *);
void mylistdir (char *);

FILE *outf;

int doccount=1000;
int doccount2=10000;

void mylistdir (char *path) 
{
  	DIR *dir;
	struct dirent *entry;
	struct stat sb;
	char statcheck [1024];
	char newpath[1024];
	char newpathf[1024];
	char extfile [1024];
	dir = opendir(path);
	int len, lennew, lennewlast;
	if(dir == 0)
	{
		return;
	}
//	printf ("* %s", path);
	while(entry = readdir(dir))
	{	
//		printf ("%s\%s\n",path, entry->d_name);
		if(entry->d_type == DT_DIR)
		{
			if (strcmp (entry->d_name, ".") != 0 && strcmp (entry->d_name, "..") != 0) 
			{
				strcpy (newpath, path);
				len = strlen (newpath);
				if (newpath [len-1] != '/')
					strcat (newpath, "/");
				strcat (newpath, entry->d_name);
				mylistdir (newpath);
			}

		}
		else
		{
			strcpy (newpathf, path);
			len = strlen (newpathf);
			if (newpathf [len-1] != '/')
				strcat (newpathf, "/");
			strcat (newpathf, entry->d_name);
			lennew = strlen (newpathf);
			lennewlast = lennew;
			extfile[0] = '\n';
			int extcountsymb=0;
			
			while ((newpathf [lennew] != '.') && (newpathf [lennew] != '/') )
				lennew--;
			if (newpathf[lennew] == '/') 
				extfile[0] = '\n';
			if (newpathf[lennew] == '.')
			{	
				for (extcountsymb=0; lennew <= lennewlast; lennew++, extcountsymb++)
					extfile[extcountsymb] = newpathf[lennew];
				
			}
//			fprintf(myout,"%s\n", extfile);
			if ((strcmp (extfile ,".txt") == 0) /*|| (strcmp (extfile ,".conf") ==0)*/)
				#ifdef USE_LIBEXPAT
					myxmlpipe2 (newpathf);
				#else
					myxmlpipe (newpathf);
				#endif

		}

	}
	closedir(dir);
}

void getworsfromfile (char *filename)
{
	FILE *f;
	char c;
	f = fopen (filename, "r");
	if (!f)
		return;
	while ( (c=getc(f)) != EOF )
	{
		putc(c, stdout);
	}
	fclose (f);
}

void createxmlpipe (void)
{
	char *xmldochead = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\
<sphinx:docset>\n\
<sphinx:schema>\n\
<sphinx:field name=\"content\"/> \n\
<sphinx:attr name=\"filename\" type=\"string\"/> \n\
<sphinx:attr name=\"lastmodified\" type=\"timestamp\"/> \n \
<sphinx:attr name=\"lastaccess\" type=\"timestamp\"/> \n \
</sphinx:schema>\n\
\n";

//<sphinx:attr name=\"lastmodified\" type=\"timestamp\"/> \n\

	fout (xmldochead);
}

void closexmlpipe (void)
{
	char *xmldocend = "</sphinx:docset>\n";
	fout (xmldocend);
}	

void myxmlpipe2 (char *filename)
{
	FILE *f;
	struct stat sb;
	char c;

	stat (filename, &sb);
	f = fopen (filename, "r");
	if (!f)
		return;
	
	char *text = malloc (strlen(filename) + 1024);// [1024];
	char docnum [1024];
	strcpy (text, "<sphinx:document id=\"");
	sprintf(docnum, "%d", doccount);
	strcat (text, docnum);
	strcat (text, "\">\n");
	fout (text); // doc id
//		<sphinx:attr name=\"filename\" type=\"string\"/> \n\

	strcpy (text, "<filename>");
	strcat (text, filename);
	strcat (text, "</filename>\n");
	fout (text); // filename
	sprintf (docnum, "%d", sb.st_mtime);
	strcpy (text, "<lastmodified>");
	strcat (text, docnum);
	strcat (text, "</lastmodified>\n");
	fout (text); // lastmodified

	sprintf (docnum, "%d", sb.st_atime);
	strcpy (text, "<lastaccess>");
	strcat (text, docnum);
	strcat (text, "</lastaccess>\n");
	fout (text); // lastmodified

	strcpy (text, "<content>\n ![CDATA[");
	fout (text);
	while ((c=getc(f)) != EOF )
	{
//		if ((c != '<') && (c != '>') && (c != '&') && (c != '\') && ( c != '/') || (c >='0' && c <='9') || (c>='A' && c<='z') 				|| (c>='А' && c<='я') || (c == ' ')) 
		if (isalpha(c) || isalnum (c) || isspace(c) || (c== '\n'))
			fprintf (myout, "%c", c);	
	}
	strcpy (text, "\n]]</content>\n");
	fout (text);
	strcpy (text, "</sphinx:document>\n \n");
	fout (text);
	doccount++;
}

void myxmlpipe (char *filename)
{
	FILE *f;
	struct stat sb;
	int c;

	stat (filename, &sb);
	f = fopen (filename, "r");
	if (!f)
		return;

	char text [1024];
	char docnum [1024];
	strcpy (text, "<document>\n");
	fout (text); //

	sprintf(text, "<id>%d</id> \n", doccount);
	fout (text); // doc id

	sprintf(text, "<group>%d</group> \n", doccount);
	fout (text); // doc id


	sprintf (text, "<timestamp>%d</timestamp>\n", sb.st_mtime);
	fout (text); // lastmodified

	sprintf (text, "<title>%s</title>\n", filename);
	fout (text); // doc id

	strcpy (text, "<body>\n \n");
	fout (text);

	while ((c=getc(f)) != EOF )
	{
		if ((c != '<') && (c != '>') && (c != '&') && (c != '\\') && ( c != '/') || (c >='0' && c <='9') || (c>='A' && c<='z')|| (c>='А' && c<='я') || (c == ' '))
//		if (isalpha(c) || isalnum (c) || isspace(c) )
			fprintf (myout, "%c", c);
	}
	strcpy (text, "\n</body>\n");
	fout (text);
	strcpy (text, "</document>\n \n");
	fout (text);
	doccount++;
}

int main(int argc, char **argv)
{

	outf = fopen ("out.xml","w");
	char c = '0';
	if(argc = 2)
	{
		char *p = malloc (strlen(argv[1]) + 2);
		if (argv[1])
			strcpy (p, argv[1]);
		else
			;
			//strcpy (p, "/home/volodymyr/git/zsphinx/2/sphinx-2.0.6-release_t/zsphinx/src/data");

//		printf("%s\n", p);

		//printf("Using directory %s\n", p);
		//while ((c = getc (stdin)) != 's')
			//sleep (100);
		#ifndef USE_LIBEXPAT
		{
			mylistdir (p);
		}
		#else
		{
			createxmlpipe ();
			mylistdir (p);
			closexmlpipe ();
		}
		#endif
	}
	fclose (outf);
	return 0;
}
