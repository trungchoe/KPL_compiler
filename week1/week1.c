#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct
{
    char s[20];
    int sl;
    char line[100];
}data;
typedef struct nodetype
{
    data key;
    struct nodetype *right,*left;
}node;
typedef struct nodetype *tree;
void insertnode(tree *root,data x)
{
    if((*root)==NULL)
    {
        *root=(node*)malloc(sizeof(node));
        (*root)->key=x;
        (*root)->right=NULL;
        (*root)->left=NULL;
    }
    else if(strcmp(x.s,((*root)->key).s)>0) insertnode(&(*root)->right,x);
    else if(strcmp(x.s,((*root)->key).s)<0) insertnode(&(*root)->left,x);
    else{
        ((*root)->key).sl++;
        if(strstr(((*root)->key).line,x.line)==NULL) strcat(((*root)->key).line,x.line);
    }
}
int check(char s[])
{
    int i=0;
    while(i<strlen(s)){
        if(isdigit(s[i])) return 0;
        else i++;
    }
    return 1;
}
void xoa(char *xau)
{
    int i,k,j;
    k=strlen(xau);
    for(j=0;j<strlen(xau);j++)
        {
            if('A'<=xau[j]&&xau[j]<='Z') xau[j]=xau[j]+'a'-'A';
        }
    while(!isalpha(xau[0]))
        {
            for(i=0;i<k-1;i++) xau[i]=xau[i+1];
            xau[k-1]='\0';
            k=strlen(xau);
        }
    while(!isalpha(xau[k-1]))
        {
            xau[k-1]='\0';
            k=strlen(xau);
        }

}
tree search(tree root,char *x)
{
    if(root==NULL) return NULL;
    else if(strcmp(x,(root->key).s)>0) search(root->right,x);
    else if(strcmp(x,(root->key).s)<0) search(root->left,x);
    else return root;
}
void ingiua(tree root)
{
    if (root!=NULL)
    {
        ingiua(root->left);
        printf("\n%-20s%-2i%s",(root->key).s,(root->key).sl,(root->key).line);
        ingiua(root->right);
    }
}
int main(void)
{

    char buff[500], temp[50];
    int i,len, count=0;
    data h;
    tree cay=NULL, sw=NULL;
    FILE *fp,*fs;
    char c;
    char *p;
    fs=fopen("StopWord.txt","r");
    while(1){
        fscanf(fs,"%s",buff);
        strcpy(h.s,buff);
        h.sl=0;
        insertnode(&sw,h);
        if(feof(fs)) break;
    }
    fp=fopen("vanban.txt","r");
    while(1){
        fgets(buff,500,fp);
        count++;
        if(feof(fp)) break;
        len= strlen(buff);
        buff[len-1]='\0';
        p=strtok (buff," ");
        if(check(p)==1){
        xoa(p);
        strcpy(h.s,p);
        strcpy(temp,p);
        h.sl=1;
        sprintf(h.line,",%d ",count);
        if(search(sw,h.s)==NULL) insertnode(&cay,h);
        }
        p=strtok(NULL," \t");
        while (p != NULL){
            if('A'<=p[0] && p[0]<='Z'){
                if(temp[strlen(temp)-1]=='.' || temp[strlen(temp)-1]=='!' || temp[strlen(temp)-1]=='?'){
                    if(check(p)==1){
                    xoa(p);
                    strcpy(h.s,p);
                    h.sl=1;
                    sprintf(h.line,",%d ",count);
                    if(search(sw,h.s)==NULL) insertnode(&cay,h);
                    }
                }
            }
            else{
                if(check(p)==1){
                xoa(p);
                strcpy(h.s,p);
                h.sl=1;
                sprintf(h.line,",%d ",count);
                if(search(sw,h.s)==NULL) insertnode(&cay,h);
                }
            }
            strcpy(temp,p);
            p = strtok(NULL," \t");

        }
    }
    fclose(fp);
    fclose(fs);
    ingiua(cay);
    free(sw);
    free(cay);
    return 0;
}

