/**
 * @file    grafiks.h
 * @author  Bhavuk Sikka
 * 
 * @brief   This module handles various things. Creation of points and sections,
 * 			initializing terminal's parameters, drawing lines, printing text into
 * 			a defined section (text box), and changing the colors of all these things.
 * */

#include "grafiks.h"

extern int errno;
struct termios initial;
struct winsize size;
int fileno(FILE *f);
int lay=0;
static void print_text(const char*,int,int,int,int,short);
pthread_mutex_t prints;

struct pts{
    int row, col;
};

struct section{
    pts *upleft;
	int rows, cols;
    kolor *color;
};

struct kolor{
	short bg_color, fg_color;
};

int* get_winsize(){
    int* szz;
    if(!(szz=(int*)malloc(2*sizeof(int)))) return NULL;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    szz[0]=size.ws_row;
    szz[1]=size.ws_col;
    return szz;
}

section **paint_layout(const short marco, kolor *k){
	int i,j,*rc,aux;
	section **sec;
	short fl2=0;
	pts *p;
	if(marco<0||marco>2) return NULL;
	if(!(rc=get_winsize())) return NULL;
	lay=marco;
	if(create_point(&p)==ERROR){
		free(rc);
		return NULL;
	}
	if(!k){
		if(create_kolor(&k)==ERROR){
			free(rc);
			free_point(&p);
			return NULL;
		}
		k->fg_color=DEFAULT_FG;
		k->bg_color=DEFAULT_BG;
		fl2=1;
	}
	apply_kolor(k);
	if(!(sec=(section**)malloc(sizeof(section*)*abs((int)2*marco-1)))){
		free(rc);
		free_point(&p);
		if(fl2) free_kolor(&k);
		return NULL;
	}
	for (i=0;i<abs(2*marco-1);i++){
		if(create_section(&sec[i])==ERROR){
			for(j=0;j<i;j++){
				free_section(&sec[j]);
			}
			free_point(&p);
			if(fl2) free_kolor(&k);
			free(sec);
			free(rc);
			return NULL;
		}
	}

	
	switch (marco){
	case 2:
		pthread_mutex_lock(&prints);
		aux=rc[1]/3;
		i=1;
		move_cursor(1,1);
		for(j = 1; j <= rc[1]; j++){
			if(j==1) printf("╔");
			else if(j == rc[1]) printf("╗");
			else if(j==aux||j==rc[1]-aux) printf("╦");
			else printf("═");
		}
		move_cursor(rc[0],1);
		for(j = 1; j <= rc[1]; j++){
			if(j == 1) printf("╚");
			else if(j == rc[1]) printf("╝");
			else if(j==aux||j==rc[1]-aux) printf("╩");
			else printf("═");
		}
		i=1;
		while(i<=rc[1]){
			for(j = 2; j < rc[0]; j++){
				move_cursor(j,i);
				printf("║");
			}
			if(i==1) i=aux;
			else if(i==aux) i=rc[1]-aux;
			else if(i==rc[1]-aux) i=rc[1];
			else i=rc[1]+1;
		}
		pthread_mutex_unlock(&prints);
		lay=2;
		if (set_point(p,2,2)==ERROR||set_section(sec[0],p,rc[0]-2,aux-2,k)==ERROR||
			set_point(p,2,aux+1)==ERROR||set_section(sec[1],p,rc[0]-2,rc[1]-2*aux-1,k)==ERROR||
			set_point(p,2,rc[1]-aux+1)==ERROR||set_section(sec[2],p,rc[0]-2,aux-1,k)==ERROR){
			free(rc);
			free_point(&p);
			if(fl2) free_kolor(&k);
			for (i=0;i<abs(2*marco-1);i++){
				free_section(&sec[i]);
			}
			free(sec);
			fflush(stdout);
			return NULL;
		}
		break;
	case 1:
		pthread_mutex_lock(&prints);
		move_cursor(1,1);
		for(j = 1; j <= rc[1]; j++){
			if(j==1) printf("╔");
			else if(j == rc[1]) printf("╗");
			else printf("═");
		}
		move_cursor(rc[0],1);
		for(j = 1; j <= rc[1]; j++){
			if(j == 1) printf("╚");
			else if(j == rc[1]) printf("╝");
			else printf("═");
		}
		i=1;
		while(i<=rc[1]){
			for(j = 2; j < rc[0]; j++){
				move_cursor(j,i);
				printf("║");
			}
			if(i==1) i=rc[1];
			else i=rc[1]+1;
		}
		pthread_mutex_unlock(&prints);
		lay=1;
		if(set_point(p,2,2)==ERROR||set_section(sec[0],p,rc[0]-2,rc[1]-2,k)==ERROR){
			free(rc);
			free_point(&p);
			if(fl2) free_kolor(&k);
			free_section(&sec[0]);
			free(sec);
			fflush(stdout);
			return NULL;
		}
		break;
	
	default:
		lay=0;
		if(set_point(p,1,1)==ERROR||set_section(sec[0],p,rc[0],rc[1],k)==ERROR){
			free(rc);
			free_point(&p);
			if(fl2) free_kolor(&k);
			free_section(&sec[0]);
			free(sec);
			return NULL;
		}
		break;
	}
	free(rc);
	free_point(&p);
	if(fl2)	free_kolor(&k);
	reset_kolor();
	fflush(stdout);
	return sec;
}

void init_terminal() {
	struct termios new;
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;	              /*here we are setting up new. This line tells to stop the 
						canonical mode (which means waiting for the user to press 
						enter before sending)*/
	new.c_lflag &= ~ECHO;                 /*by deactivating echo, we tell the terminal NOT TO 
						show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;                  /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;	              /*I really have no clue what this does, it must be somewhere in the book tho*/
	/*new.c_lflag &= ~ISIG;*/                 /*here we discard signals: the program won't end even if we 
						press Ctrl+C or we tell it to finish*/
	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
	pthread_mutex_init(&prints,NULL);
	clear_screen();
	hide_cursor();
}

void close_terminal() {
	tcsetattr(fileno(stdin), TCSANOW, &initial);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
	pthread_mutex_destroy(&prints);
	show_cursor();
	reset_kolor();
}

void clear_stdin(){
	int stdin_copy = dup(STDIN_FILENO);
	/* remove garbage from stdin */
	tcdrain(stdin_copy);
	tcflush(stdin_copy, TCIFLUSH);
	close(stdin_copy);
}

status create_kolor(kolor **k){
	if(!k) return ERROR;
	if(!(*k=(kolor*)malloc(sizeof(kolor)))) return ERROR;
	return OK;
}

short get_kolor_fg(const kolor* k){
	if(!k) return -1;
	return k->fg_color;
}

short get_kolor_bg(const kolor* k){
	if(!k) return -1;
	return k->bg_color;
}

status set_kolor(kolor *color, const int fg_color, const int bg_color){
	if(!color) return ERROR;
	if(bg_color>255 || fg_color>255 || bg_color<0 || fg_color<0) return ERROR;
	color->fg_color=fg_color;
	color->bg_color=bg_color;
	return OK;
}

void free_kolor(kolor **k){
	if(!k) return;
	if(*k){ 
		free(*k);
		*k=NULL;
	}
}

void apply_kolor(kolor *k){
	if(!k) return;
	printf("%c[38;5;%dm%c[48;5;%dm",27,k->fg_color,27,k->bg_color);
}

void reset_kolor(){
	printf("%c[0m",27);
}

status create_point(pts**p){
	if(!p) return ERROR;
	if(!(*p=(pts*)calloc(sizeof(pts),1))) return ERROR;
	return OK;
}

status copy_point(pts **dst, const pts *src){
	if(!dst||!src) return ERROR;
	if(create_point(dst)==ERROR) return ERROR;
	set_point(*dst,src->row,src->col);
	return OK;
}

int cmp_point(const pts *p1, const pts *p2){
	if(!p1 || !p2) return __INT_MAX__;
	if (p1->col == p2->col){
		return p1->row - p2->row;
	}
	return p1->col - p2->col;
}

int get_point_row(const pts *p){
	if(!p) return 0;
	return p->row;
}

int get_point_col(const pts *p){
	if(!p) return 0;
	return p->col;
}

status set_point(pts *point, const int row, const int col){
	if(!point) return ERROR;
	point->row=row;
	point->col=col;
	return OK;
}

void free_point(pts **p){
	if(!p) return;
	if(*p) {
		free(*p);
		*p=NULL;
	}
}

status create_section(section **sec){
	if(!sec) return ERROR;
	if(!(*sec=(section*)calloc(sizeof(section),1))) return ERROR;
	if(create_kolor(&(*sec)->color)==ERROR){
		free(sec);
		return ERROR;
	}
	if(create_point(&(*sec)->upleft)==ERROR){
		free_kolor(&((*sec)->color));
		free(sec);
		return ERROR;
	}
	return OK;
}

pts* get_section_point(const section *sec){
	pts* p;
	if(!sec) return NULL;
	if(copy_point(&p, sec->upleft)==ERROR) return NULL;
	return p;
}

int get_section_row(const section *sec){
	if(!sec) return -1;
	return sec->rows;
}

int get_section_col(const section *sec){
	if(!sec) return -1;
	return sec->cols;
}

kolor *get_section_kolor(const section *sec){
	kolor *k;
	if(!sec) return NULL;
	if(create_kolor(&k)==ERROR) return NULL;
	k->bg_color=sec->color->bg_color;
	k->fg_color=sec->color->fg_color;
	return k;
}

status set_section(section *sec, const pts *p, const int rows, const int cols, kolor *k){
	int *rc,aux;
	if(!sec||!p||p->col<1||p->row<1||rows<1||cols<1) return ERROR;
	if(!(rc=get_winsize())) return ERROR;
	if(rows+p->row-1>rc[0] || cols+p->col-1>rc[1]){
		free(rc);
		return ERROR;
	}
				
	switch (lay){
	case 2:
		aux=rc[1]/3;
		if((p->col<=aux&&cols+p->col-1>=aux)||(p->col<=rc[1]-aux&&cols+p->col>rc[1]-aux)||p->col>=rc[1]||p->col==1||
			p->row>=rc[0]||p->row==1||p->col==aux){
				free(rc);
			return ERROR;
		}
		break; 
	case 1:
		aux=rc[1]/3;
		if (p->col>=rc[1]||p->col==1||p->row>=rc[0]||p->row==1){
			free(rc);
			return ERROR;
		}
		break;
	default:
		if(p->col>rc[1]||p->row>rc[0]){
			free(rc);
			return ERROR;
		}
		break;
	}
	sec->cols=cols;
	sec->rows=rows;
	sec->upleft->col=p->col;
	sec->upleft->row=p->row;
	if(!k) set_kolor(sec->color,DEFAULT_FG,DEFAULT_BG);
	else set_kolor(sec->color,k->fg_color,k->bg_color);
	free(rc);
	return OK;
}

void clear_section(section *sec){
	int i;
	pts *p;
	char *c, aux=' ';
	if(!sec) return;
	if(!(c=(char*)malloc(sec->cols+1))) return;
	if(!(p=get_section_point(sec))){
		free(c); return;
	}
	memset(c,aux,sec->cols);
	c[sec->cols]=0;
	for (i=0;i<sec->rows;i++){
		draw_line(p,p,c,sec,NULL,0);
		set_point(p,get_point_row(p)+1,get_point_col(p));
	}
	free_point(&p);
	free(c);
	fflush(stdout);
}

void fill_section(section *sec, kolor *k){
	pts *p,*p2;
	int i;
	char *str;
	if(!sec) return;
	if(!k){
		clear_section(sec);
		return;
	}
	if(!(str=(char*)malloc(get_section_row(sec)*3+1))) return;
	if(!(p=get_section_point(sec))){
		free(str);
		return;
	} 
	if(!(p2=get_section_point(sec))){
		free_point(&p);
		free(str);
		return;
	}
	str[0]=0;
	set_point(p2,get_point_row(p2),get_point_col(p2)+get_section_col(sec)-1);
	for (i=0;i<get_section_row(sec);i++){
		strcat(str,"█");
	}

	draw_line(p,p2,str,sec,k,1);
	free_point(&p);
	free_point(&p2);
	free(str);
}

bool in_section(const section *sec,const pts *p){
	if(!p||!sec) return FALSE;
	if((sec->upleft->row<=p->row && p->row<sec->rows+sec->upleft->row)&&
	(sec->upleft->col<=p->col && p->col<sec->cols+sec->upleft->col)) return TRUE;
	return FALSE;
}

void free_section(section **sec){
	if(!sec) return;
	if (*sec){
		free_point(&((*sec)->upleft));
		free_kolor(&((*sec)->color));
		free(*sec);
		*sec=NULL;
	}
}

status textbox(const char *str, const section* sec, const int al, kolor *k){
	int i=0, actsz=0, futsz=0, row=0, pastpos=-1,j;
	short flag=1,fl2=0,aux=0;
	pts *p;
	/*ERROR CHECKING AND ALLOCS*/
	if(!str||!sec||al<-1||al>1) return ERROR;
	if(!(p=get_section_point(sec))) return ERROR;
	if(!k){
		if(!(k=get_section_kolor(sec))){
			free_point(&p);
			return ERROR;
		}
		fl2=1;
	}
	/*NOW STARTS THE REAL SHIT*/
	pthread_mutex_lock(&prints);
	apply_kolor(k);
	move_cursor(2,2);
	while(i<strlen(str)+1 && flag){
		/*if(str[i]==-61||str[i]==-62){
			aux++;
			-61 ES EL CARACTER QUE SE PONE ANTES DE LA Ñ Á É Í Ó Ú
		}*/
		if((unsigned char)str[i]>=0xc0&&(unsigned char)str[i]<=0xdf){
			aux+=1;
		}else if((unsigned char)str[i]>=0xe0&&(unsigned char)str[i]<=0xef){
			aux+=2;
		}else if((unsigned char)str[i]>=0xf0&&(unsigned char)str[i]<=0xf7){
			aux+=3;
		}
		if(str[i]==' '||str[i]=='\0'){
			futsz=i;
			if(sec->rows<=row){
				flag=0;
			}else if(futsz-pastpos-aux>sec->cols){
				if(futsz-actsz-aux>sec->cols){
					free_point(&p);
					if(fl2){
						free_kolor(&k);
					}
					fprintf(stdout,"%c[0m",27);
					fflush(stdout);
					return ERROR;
				}
				move_cursor(p->row+row,p->col);
				row++;
				for(j=i-1;j>=actsz;j--){
					if(str[j]==-61) aux--;
				}
				print_text(str,pastpos+1,actsz,sec->cols,al,aux);
				aux=0;
				i=actsz;
				pastpos=i;
			}else if (str[i]=='\0'){
				if(futsz-actsz-aux>sec->cols){
					free_point(&p);
					if(k){
						free_kolor(&k);
					}
					fprintf(stdout,"%c[0m",27);
					fflush(stdout);
					return ERROR;
				}
				move_cursor(p->row+row,p->col);
				row++;
				for(j=i-1;j>=futsz;j--){
					if(str[j]==-61) aux--;
				}
				print_text(str,pastpos+1,futsz,sec->cols,al,aux);
				aux=0;
			}else{
				actsz=futsz;
			}
		}
		i++;
	}
	fflush(stdout);
	reset_kolor();
	pthread_mutex_unlock(&prints);
	/*END*/
	free_point(&p);
	if(fl2){
		free_kolor(&k);
	}
	return OK;
}

static void print_text(const char *str,int from, int to, int width, int al,short aux){
	int i=from,j=0;
	switch (al)
	{
	case -1:
		while(i<to){
			fprintf(stdout,"%c",str[i]);
			i++;
		}
		while(j<width-(to-from)+aux){
			fprintf(stdout," ");
			j++;
		}
		break;
	case 0:
		while(j<(width-(to-from)+aux)/2){
			fprintf(stdout," ");
			j++;
		}
		while(i<to){
			fprintf(stdout,"%c",str[i]);
			i++;
		}
		j=0;
		while(j<(width-(to-from)+aux+1)/2){
			fprintf(stdout," ");
			j++;
		}
		break;
	default:
		while(j<width-(to-from)+aux){
			fprintf(stdout," ");
			j++;
		}
		while(i<to){
			fprintf(stdout,"%c",str[i]);
			i++;
		}
		break;
	}
}

static void print_draw_line(char *str,short flag, const section *sec, const pts *paux){
	int i,j;
	short len;
	len=strlen(str);
	pts *p;
	if(copy_point(&p,paux)==ERROR) return;
	move_cursor(get_point_row(paux),get_point_col(paux));
	for(i=0;i<len;i++){
		j=i;
		if((unsigned char)str[i]>=0xc0&&(unsigned char)str[i]<=0xdf){
			i+=1;
		}else if((unsigned char)str[i]>=0xe0&&(unsigned char)str[i]<=0xef){
			i+=2;
		}else if((unsigned char)str[i]>=0xf0&&(unsigned char)str[i]<=0xf7){
			i+=3;
		}
		if(in_section(sec,p)==TRUE){
			for(;j<=i;j++){
				fprintf(stdout,"%c",str[j]);
			}
		}else{
			i=len;
		}
		set_point(p,get_point_row(p)+(flag!=0),get_point_col(p)+(flag==0));
		if(flag){
			move_cursor(get_point_row(p),get_point_col(p));
		}
	}
	free_point(&p);
}

status draw_line(const pts *p1,const pts*p2, char* thikk, const section* sec, kolor* k, short flag){
	int i,start,end,subeobaja=0, ic=10, aux=0, aux2=0;
	int n=1;
	double m=0,b;
	short fl=0,fl2=0;
	char ch[][4] = {"┓", "┏", "┛", "┗", "━", "┃", "┣", "┫", "┻", "┳", "╋"};
	pts *paux, *pfuture;
	if(!p1||!p2||!sec) return ERROR;
	if(create_point(&paux)==ERROR) return ERROR;
	if(create_point(&pfuture)==ERROR){ 
		free_point(&paux);
		return ERROR;
	}
	if(!k){
		if(!(k=get_section_kolor(sec))){
			free_point(&paux);
			free_point(&pfuture);
			return ERROR;
		}
		fl2=1;
	}
	if(get_point_col(p2)!=get_point_col(p1)) {
		m=(get_point_row(p2)-get_point_row(p1)+.0)/(get_point_col(p2)-get_point_col(p1)+.0);
		b=get_point_row(p2)-get_point_col(p2)*m;
	}else {
		n=0;
	}
	if(!thikk){
		fl=1;
		if(!(thikk=(char*)malloc(4))){ 
			if (fl2) free_kolor(&k);
			free_point(&paux);
			free_point(&pfuture);
			return ERROR;
		}
	}

	pthread_mutex_lock(&prints);
	apply_kolor(k);
	if (fl==1) strcpy(thikk,ch[10]);
	set_point(paux,get_point_row(p1),get_point_col(p1));
	print_draw_line(thikk,flag,sec,p1);

	if(n){
		start=get_point_col(p1);
		end=get_point_col(p2);
		subeobaja = 2*(end>start) -1;
		/*p1->col*(p2->col>p1->col)+p2->col*(p1->col>p2->col)*/
		/*p1->col*(p2->col<p1->col)+p2->col*(p1->col<p2->col)*/
		for (i=start+subeobaja;i!=(end+subeobaja);i+=subeobaja){
			set_point(pfuture,(int)(m*i+b+0.5),i);
			aux2=0;
			if(fl==1){
				if(cmp_point(paux,pfuture)>0){
					set_point(paux,get_point_row(paux),i);
					if(cmp_point(paux,pfuture)>0){
						ic=3;
						aux=0;
					}else{
						ic=1;
						aux=2;
					}
				}else{
					set_point(paux,get_point_row(paux),i);
					if(cmp_point(paux,pfuture)>0){
						ic=2;
						aux=1;
					}else{
						ic=0;
						aux=3;
					}
				}
				strcpy(thikk,ch[ic]);
			}
			set_point(paux,get_point_row(paux),i);
			while(cmp_point(paux, pfuture)!=0){
				print_draw_line(thikk,flag,sec,paux);
				set_point(paux,get_point_row(paux)-(2*(m<0)-1)*subeobaja,get_point_col(paux));
				if(fl==1){
					ic=5;
					strcpy(thikk,ch[ic]);
				}
				aux2=1;
			}
			if(fl==1){
				if(cmp_point(paux,p2)==0){
					ic=10;
				}else if(aux2==0){
					ic=4;
				}else{
					ic=aux;
				}
				strcpy(thikk,ch[ic]);
			}
			print_draw_line(thikk,flag,sec,paux);
		}
	}else{
		start=get_point_row(p1);
		end=get_point_row(p2);
		subeobaja = 2*(end>start) -1;
		/*p1->row*(p2->row>=p1->row)+p2->row*(p1->row>p2->row)*/
		/*p1->row*(p2->row<=p1->row)+p2->row*(p1->row<p2->row)*/
		set_point(pfuture,get_point_row(p2),get_point_col(p2));
		if(fl==1){
			ic=5;
			strcpy(thikk,ch[ic]);
		}
		while(cmp_point(paux, pfuture)!=0){
			set_point(paux,get_point_row(paux)+subeobaja,get_point_col(paux));
			print_draw_line(thikk,flag,sec,paux);
		}
		if(fl==1){
			ic=10;
			strcpy(thikk,ch[ic]);
		}
		print_draw_line(thikk,flag,sec,paux);
	}

	fflush(stdout);
	reset_kolor();
	pthread_mutex_unlock(&prints);
	if (fl) free(thikk);
	if (fl2) free_kolor(&k);
	free_point(&paux);
	free_point(&pfuture);
	return OK;
}
