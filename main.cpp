#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <math.h>
#include <string>
using namespace std;
int numPunto=0;
int pxi=10;
int pyi=50;
int pxf=610;
int pyf=650;
struct punto{
    float x;
    float y;
    punto(){x=0;y=0;}
    float distancia(punto p){
        return sqrt (pow(p.x-x, 2) + pow(p.y-y, 2));
    }
    punto(float x,float y){
        this->x=x;
        this->y=y;
    }
};
HINSTANCE hInst;
class Nodo
{   public:
    punto B1;
    punto B2;
    punto PL;
    punto PR;
    float LRADIUS;
    float RRADIUS;
    float DL;
    float DR;
    bool WENTRIGHT;
    bool FULL;
    bool RadioL;
    bool RadioR;
    int NumL;
    int NumR;
    Nodo *LCHILD,*RCHILD,*ANCESTOR,*TEMPSTORAGE;
    Nodo()
    {
        LCHILD=NULL;
        RCHILD=NULL;
        ANCESTOR=NULL;
        TEMPSTORAGE=NULL;
        LRADIUS=0;
        RRADIUS=0;
        RadioL=false;
        RadioR=false;
        FULL=false;
    }
};

class Bisector{
  private:
    Nodo *Raiz;
    char DIRECTION;
    float NDIST;
  public:
    punto q;
    punto radio;
    Bisector()
    {   Raiz=NULL;
        radio=punto(0,0);
    }
    void Insertar(punto p)
    {
        Ins(p,Raiz,Raiz);
    }
    void Ins(punto p,Nodo *&V,Nodo *W)
    {
        if(V==NULL){
            V=new Nodo();
            V->PL=p;
            V->FULL=false;
            V->ANCESTOR=W;
            V->NumL=numPunto;
        }
        else if(V->FULL==false){
            V->PR=p;
            V->FULL=true;
            V->NumR=numPunto;
            evaluar(V,W);
        }
        else{
                if(p.distancia(V->PL)<=p.distancia(V->PR)){
                    if(p.distancia(V->PL)>V->LRADIUS)
                        V->LRADIUS=p.distancia(V->PL);
                    Ins(p,V->LCHILD,V);
                }
                else{
                    if(p.distancia(V->PR)>V->RRADIUS)
                        V->RRADIUS=p.distancia(V->PR);
                    Ins(p,V->RCHILD,V);
                }
        }
    }
    MostrarGrafico(HDC hdc)
    {
        MosG(hdc,Raiz);
    }
    MosG(HDC hdc,Nodo *R)
    {
        if(R!=NULL)
        {
            char Cad[20];
            if(R->FULL){
                HBRUSH verde,azul;
                verde=CreateSolidBrush(RGB(17,171,22));
                azul=CreateSolidBrush(RGB(7,71,121));
                sprintf(Cad,"%i",R->NumL);
                TextOut(hdc,R->PL.x-7,R->PL.y-22,Cad,strlen(Cad));
                SelectObject(hdc,verde);
                Ellipse(hdc, R->PL.x-7, R->PL.y-7, R->PL.x+7,R->PL.y+7);
                SelectObject(hdc,azul);
                sprintf(Cad,"%i",R->NumR);
                TextOut(hdc,R->PR.x-7,R->PR.y-22,Cad,strlen(Cad));
                Ellipse(hdc, R->PR.x-7, R->PR.y-7, R->PR.x+7,R->PR.y+7);
                MoveToEx(hdc,R->B1.x,R->B1.y,NULL);
                LineTo(hdc,R->B2.x,R->B2.y);
                if(R->LRADIUS>0&&radio.x>R->PL.x-7&&radio.x<R->PL.x+7&&radio.y>R->PL.y-7&&radio.y<R->PL.y+7){
                    if(R->RadioL)
                        R->RadioL=false;
                    else
                        R->RadioL=true;
                }
                if(R->RRADIUS>0&&radio.x>R->PR.x-7&&radio.x<R->PR.x+7&&radio.y>R->PR.y-7&&radio.y<R->PR.y+7){
                    if(R->RadioR)
                        R->RadioR=false;
                    else
                        R->RadioR=true;
                }
                if(R->RadioL){
                    Arc(hdc,
                    R->PL.x-R->LRADIUS,R->PL.y-R->LRADIUS,
                    R->PL.x+R->LRADIUS,R->PL.y+R->LRADIUS,
                    R->PL.x-R->LRADIUS,R->PL.y,
                    R->PL.x-R->LRADIUS,R->PL.y);
                }
                if(R->RadioR){
                    Arc(hdc,
                    R->PR.x-R->RRADIUS,R->PR.y-R->RRADIUS,
                    R->PR.x+R->RRADIUS,R->PR.y+R->RRADIUS,
                    R->PR.x-R->RRADIUS,R->PR.y,
                    R->PR.x-R->RRADIUS,R->PR.y);
                }
                DeleteObject(verde);
                DeleteObject(azul);
            }
            else{
                HBRUSH negro;
                negro=CreateSolidBrush(RGB(0,0,0));
                sprintf(Cad,"%i",R->NumL);
                TextOut(hdc,R->PL.x-7,R->PL.y-22,Cad,strlen(Cad));
                SelectObject(hdc,negro);
                Ellipse(hdc, R->PL.x-7, R->PL.y-7, R->PL.x+7,R->PL.y+7);
                DeleteObject(negro);
            }
            MosG(hdc,R->LCHILD);
            MosG(hdc,R->RCHILD);
        }
    }
    void MostrarTree(HDC hdc,int xi,int yi,int xf,int yf)
    {
        MosT(hdc,Raiz,(xi+xf)/2,yi,xf-xi);
    }
    void MosT(HDC hdc,Nodo *R,int x,int y,int a)
    {
         if(R!=NULL)
        {
            char Cad[20];
            int num;
            if(R->FULL){
                HBRUSH verde, azul;
                verde=CreateSolidBrush(RGB(17,171,22));
                azul=CreateSolidBrush(RGB(7,71,121));
                num=R->PL.x;
                sprintf(Cad,"%i",R->NumL);
                TextOut(hdc,x+14,y+14,Cad,strlen(Cad));
                SelectObject(hdc,verde);
                Ellipse(hdc, x, y+15, x+14,y+29);
                sprintf(Cad,"%i",num);
                TextOut(hdc,x,y+30,Cad,strlen(Cad));
                num=R->PL.y;
                sprintf(Cad,"%i",num);
                TextOut(hdc,x,y+45,Cad,strlen(Cad));
                sprintf(Cad,"%i",R->NumR);
                TextOut(hdc,x+44,y+14,Cad,strlen(Cad));
                SelectObject(hdc,azul);
                Ellipse(hdc, x+30, y+15, x+44,y+29);
                num=R->PR.x;
                sprintf(Cad,"%i",num);
                TextOut(hdc,x+30,y+30,Cad,strlen(Cad));
                num=R->PR.y;
                sprintf(Cad,"%i",num);
                TextOut(hdc,x+30,y+45,Cad,strlen(Cad));
                DeleteObject(verde);
                DeleteObject(azul);
            }else{
                HBRUSH negro;
                negro=CreateSolidBrush(RGB(0,0,0));
                num=R->PL.x;
                sprintf(Cad,"%i",R->NumL);
                TextOut(hdc,x+14,y+14,Cad,strlen(Cad));
                SelectObject(hdc,negro);
                Ellipse(hdc, x, y+15, x+14,y+29);
                sprintf(Cad,"%i",num);
                TextOut(hdc,x,y+30,Cad,strlen(Cad));
                num=R->PL.y;
                sprintf(Cad,"%i",num);
                TextOut(hdc,x,y+45,Cad,strlen(Cad));
                DeleteObject(negro);
            }
            MosT(hdc,R->LCHILD,x-a/10,y+80,a/2);
            MosT(hdc,R->RCHILD,x+a/10,y+80,a/2);
        }
    }
    /* Halla la ecuacion de la recta del bisector de los puntos a y b
        Retorna un punto del bisector, cuando X = C */
    punto puntoX(punto a,punto b,int c)
    {
        punto p;
        punto medio;
        punto pendiente;
        float B;
        medio.x=(a.x+b.x)/2;
        medio.y=(a.y+b.y)/2;
        pendiente.y=b.y-a.y;
        pendiente.x=b.x-a.x;
        B=medio.y-(-1*(pendiente.x/pendiente.y)*medio.x);
        p.x=c;
        p.y=(-1*(pendiente.x/pendiente.y)*p.x)+B;
        return p;
    }
    /* Halla la ecuacion de la recta del bisector de los puntos a y b
        Retorna un punto del bisector, cuando Y = C */
    punto puntoY(punto a,punto b,int c)
    {
        punto p;
        punto medio;
        punto pendiente;
        medio.x=(a.x+b.x)/2;
        medio.y=(a.y+b.y)/2;
        if(a.y!=b.y){
            float B;
            pendiente.y=b.y-a.y;
            pendiente.x=b.x-a.x;
            B=medio.y-(-1*(pendiente.x/pendiente.y)*medio.x);
            p.y=c;
            p.x=(p.y-B)/(-1*(pendiente.x/pendiente.y));
        }else{
            p.x=medio.x;
            p.y=c;
        }
        return p;
    }
    /* Retorna el Punto donde se cruzan la recta ab y cd */
    punto punto_cruce(punto a,punto b,punto c,punto d)
    {
        punto p;
        float m1=(b.y-a.y)/(b.x-a.x);
        float x1=m1;
        float y1;
        float B1;
        B1=m1*-a.x+a.y;
        float m2=(d.y-c.y)/(d.x-c.x);
        float x2=m2;
        float y2;
        float B2;
        B2=m2*-c.x+c.y;
        if(a.x==b.x&&c.x==d.x){
            p.x=c.x;
            p.y=d.x;
        }
        else if(c.x==d.x){
            p.x=c.x;
            p.y=m1*(p.x-a.x)+a.y;
        }
        else if(a.x==b.x){
            p.x=a.x;
            p.y=m2*(p.x-c.x)+c.y;
        }
        else{
            p.x=(B2-B1)/(x1-x2);
            p.y=(x1*p.x)+B1;
        }
        return p;
    }
    /* Evalua el tamaño de la Recta del Bisector del nuevo nodo insertado, con respecto a sus ancestros.*/
    void evaluar(Nodo*V,Nodo*W){
            punto medi;
            punto p1;
            punto p2;
            punto p_aux;
            medi.x=(V->PL.x+V->PR.x)/2;
            medi.y=(V->PL.y+V->PR.y)/2;
            p1=puntoX(V->PL,V->PR,pxi);
            p2=puntoX(V->PL,V->PR,pxf);
            if(!(p1.y>=pyi&&p1.y<=pyf)){
                p1=puntoY(V->PL,V->PR,pyi);
                if(!(p1.x>=pxi&&p1.x<=pxf))
                    p1=puntoY(V->PL,V->PR,pyf);
            }
            if(!(p2.y>=pyi&&p2.y<=pyf)){
                p2=puntoY(V->PL,V->PR,pyf);
                if(!(p2.x>=pxi&&p2.x<=pxf))
                    p2=puntoY(V->PL,V->PR,pyi);
            }
            p_aux=punto_cruce(p1,p2,Raiz->B1,Raiz->B2);
            if(p_aux.x==p1.x&&p_aux.x==p2.x){
                if((p_aux.y>p1.y&&p_aux.y<medi.y)||(p_aux.y<p1.y&&p_aux.y>medi.y)){
                    p1=p_aux;
                }
                else if((p_aux.y>p2.y&&p_aux.y<medi.y)||(p_aux.y<p2.y&&p_aux.y>medi.y)){
                    p2=p_aux;
                }
            }else{
                if((p_aux.x>p1.x&&p_aux.x<medi.x)||(p_aux.x<p1.x&&p_aux.x>medi.x)){
                    p1=p_aux;
                }
                else if((p_aux.x>p2.x&&p_aux.x<medi.x)||(p_aux.x<p2.x&&p_aux.x>medi.x)){
                    p2=p_aux;
                }
            }
            Nodo*aux=W;
            while(aux!=Raiz){
                p_aux=punto_cruce(p1,p2,aux->B1,aux->B2);
                if(p_aux.x==p1.x&&p_aux.x==p2.x){
                    if((p_aux.y>p1.y&&p_aux.y<medi.y)||(p_aux.y<p1.y&&p_aux.y>medi.y)){
                        p1=p_aux;
                    }
                    else if((p_aux.y>p2.y&&p_aux.y<medi.y)||(p_aux.y<p2.y&&p_aux.y>medi.y)){
                        p2=p_aux;
                    }
                }else{
                    if((p_aux.x>p1.x&&p_aux.x<medi.x)||(p_aux.x<p1.x&&p_aux.x>medi.x)){
                        p1=p_aux;
                    }
                    else if((p_aux.x>p2.x&&p_aux.x<medi.x)||(p_aux.x<p2.x&&p_aux.x>medi.x)){
                        p2=p_aux;
                    }
                }
                aux=aux->ANCESTOR;
            }
            V->B1=p1;
            V->B2=p2;
    }
    void Buscar(HDC hdc,punto z)
    {
        if(Raiz==NULL)
            return;
        DOWNTOWN(hdc,Raiz,z);
    }
    void DOWNTOWN(HDC hdc,Nodo*&p,punto z)
    {
        if(!Raiz->FULL){
                q=Raiz->PL;
                NDIST=z.distancia(q);
                UPTOWN(hdc,Raiz->ANCESTOR,z);
                return;
        }
        float DL,DR;
            if(p==Raiz){
                DL=z.distancia(p->PL);
                DR=z.distancia(p->PR);
                NDIST=min(DL,DR);
                if(NDIST==DL)
                    q=p->PL;
                else
                    q=p->PR;
            }
            if(!p->FULL){
                DL=z.distancia(p->PL);
                if(DL<NDIST){
                    NDIST=DL;
                    q=p->PL;
                }
                UPTOWN(hdc,p->ANCESTOR,z);
            }
            else{
                DL=z.distancia(p->PL);
                DR=z.distancia(p->PR);
                if(DL<NDIST){
                    NDIST=DL;
                    q=p->PL;
                }
                if(DR<NDIST){
                    NDIST=DR;
                    q=p->PR;
                }
                DECISION(DL,DR,p);
                if(DIRECTION=='U'){
                    UPTOWN(hdc,p->ANCESTOR,z);
                }
                if(DIRECTION=='L'){
                    DOWNTOWN(hdc,p->LCHILD,z);
                }
                if(DIRECTION=='R'){
                    DOWNTOWN(hdc,p->RCHILD,z);
                }
            }
    }
    void UPTOWN(HDC hdc,Nodo*p,punto z)
    {
             if(p==Raiz->ANCESTOR){
                HBRUSH rojo;
                rojo=CreateSolidBrush(RGB(189,8,8));
                SelectObject(hdc,rojo);
                Ellipse(hdc, q.x-7, q.y-7, q.x+7,q.y+7);
                Ellipse(hdc, z.x-7, z.y-7, z.x+7,z.y+7);
                DeleteObject(rojo);
                return;
             }
             if(p->TEMPSTORAGE==NULL){
                UPTOWN(hdc,p->ANCESTOR,z);
             }
             else{
                float DL,DR;
                bool WENTRIGHT;
                DL=p->TEMPSTORAGE->DL;
                DR=p->TEMPSTORAGE->DR;
                WENTRIGHT=p->TEMPSTORAGE->WENTRIGHT;
                p->TEMPSTORAGE=NULL;
                if((WENTRIGHT&&DL-p->LRADIUS>NDIST)||(!WENTRIGHT&&DR-p->RRADIUS>NDIST)){
                    UPTOWN(hdc,p->ANCESTOR,z);
                }else{
                    if(WENTRIGHT)
                        DOWNTOWN(hdc,p->LCHILD,z);
                    else
                        DOWNTOWN(hdc,p->RCHILD,z);
                }
             }
    }
    void DECISION(int DL,int DR,Nodo*p)
    {
            if(p->RRADIUS>0&&p->LRADIUS>0){
                if(DL-p->LRADIUS<NDIST&&DR-p->RRADIUS>=NDIST){
                        DIRECTION='L';
                }
                else if(DL-p->LRADIUS<NDIST&&DR-p->RRADIUS<NDIST){
                    p->TEMPSTORAGE=new Nodo();
                    p->TEMPSTORAGE->DL=DL;
                    p->TEMPSTORAGE->DR=DR;
                    if(DR<DL){
                        DIRECTION='R';
                        p->TEMPSTORAGE->WENTRIGHT=true;
                    }else{
                        DIRECTION='L';
                        p->TEMPSTORAGE->WENTRIGHT=false;
                    }
                }
                else if(DL-p->LRADIUS>=NDIST&&DR-p->RRADIUS<NDIST){
                        DIRECTION='R';
                }else if(DL-p->LRADIUS>=NDIST&&DR-p->RRADIUS>=NDIST){
                        DIRECTION='U';
                }
            }
            else if(p->LRADIUS>0){
                if(DL-p->LRADIUS<NDIST)
                    DIRECTION='L';
                else
                    DIRECTION='U';
            }
             else if(p->RRADIUS>0){
                if(DR-p->RRADIUS<NDIST)
                    DIRECTION='R';
                else
                    DIRECTION='U';
            }
            else{
                DIRECTION='U';
            }
    }
};
Bisector B;
punto buscar;
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {   case WM_PAINT:
        {   PAINTSTRUCT ps;
            HDC hdc;
            hdc=BeginPaint(hwndDlg,&ps);
            Rectangle(hdc,pxi,pyi,pxf,pyf);
            B.MostrarTree(hdc,100,50,1750,400);
            B.MostrarGrafico(hdc);
            if(buscar.x+buscar.y>0){
                B.Buscar(hdc,buscar);
                buscar=punto(0,0);
                B.q=buscar;
            }
            EndPaint(hwndDlg,&ps);
        }
        return TRUE;
        case WM_LBUTTONDOWN:
        {   long xPos=GET_X_LPARAM(lParam);
            long yPos=GET_Y_LPARAM(lParam);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi){
                    punto p;
                    p.x=xPos;
                    p.y=yPos;
                    numPunto++;
                    B.Insertar(p);
            }
            InvalidateRect(hwndDlg,NULL,true);
        }
        return TRUE;
        case WM_RBUTTONDOWN:
        {
            long xPos=GET_X_LPARAM(lParam);
            long yPos=GET_Y_LPARAM(lParam);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi){
                    B.radio=punto(xPos,yPos);
            }
            InvalidateRect(hwndDlg,NULL,true);
        }
        return TRUE;
        case WM_MBUTTONDOWN:
        {
            long xPos=GET_X_LPARAM(lParam);
            long yPos=GET_Y_LPARAM(lParam);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi){
                char Cad[20];
                sprintf(Cad,"%i",xPos);
                SetDlgItemText(hwndDlg,EDITXB,Cad);
                sprintf(Cad,"%i",yPos);
                SetDlgItemText(hwndDlg,EDITYB,Cad);
                buscar.x=xPos;
                buscar.y=yPos;
            }
            InvalidateRect(hwndDlg,NULL,true);
        }
        return TRUE;
        case WM_RBUTTONUP:
        {
            B.radio=punto(0,0);
        }
        return TRUE;
        case WM_MOUSEMOVE:
        {   long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            if(xPos<pxf&&xPos>pxi&&yPos<pyf&&yPos>pyi)
            {   char Cad[20];
                sprintf(Cad,"%i",xPos);
                SetDlgItemText(hwndDlg,200,Cad);
                sprintf(Cad,"%i",yPos);
                SetDlgItemText(hwndDlg,EDITY,Cad);
            }
        }
        return TRUE;
        case WM_INITDIALOG:
        {
        }
        return TRUE;
        case WM_CLOSE:
        {   EndDialog(hwndDlg, 0);
        }
        return TRUE;
        case WM_COMMAND:
        {   switch(LOWORD(wParam))
            {
                case BUSCAR:
                    punto p;
                    char Cad[20];
                    GetDlgItemText(hwndDlg,EDITXB,Cad,10);
                    p.x = atof(Cad);
                    GetDlgItemText(hwndDlg,EDITYB,Cad,10);
                    p.y = atof(Cad);
                    if(p.x>pxi&&p.x<pxf&&p.y>pyi&&p.y<pyf){
                        buscar=p;
                        InvalidateRect(hwndDlg,NULL,true);
                    }

                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
