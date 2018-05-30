#include<stdio.h>
#include<stdlib.h>
struct aaa
{
    int number;
    char name[50];
    int score;
};
int main1()
{
    struct aaa d[20];
    FILE *fp;
    int a, c;
    int A;
    A=sizeof(int)+sizeof(char)+sizeof(int);
    char b[50];
    int i, j;
    if ((fp=fopen("C:\\Users\\50205\\Desktop\\z.txt", "w"))!=NULL)
    {
        for (i=0;i<5;i++)
        {
            scanf("%d%s%d", &a, b, &c);
            if (i!=4)
            {
            fprintf(fp, "%d %s %d\n", a, b, c);
            }
            else
            fprintf(fp, "%d %s %d", a, b, c);
        }
    }
    else
        printf("sascas");
    fclose(fp);
    scanf("%d", &j);
    if ((fp=fopen("C:\\Users\\50205\\Desktop\\z.txt", "r"))!=NULL)
    {
        for (i=0;i<5;i++)
        {
            fscanf(fp, "%d%s%d", &d[i].number, d[i].name, &d[i].score);
        }
        for (i=0;i<5;i++)
        {
          if (d[i].number==j)
            {
               printf("%d %s %d", d[i].number, d[i].name, d[i].score);
            break;
            }
        }
    }
    else
    printf("xsxsx");
    return 0;
}
