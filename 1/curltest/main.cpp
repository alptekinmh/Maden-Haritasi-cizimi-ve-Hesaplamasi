#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <graphics.h>
#include <curl/curl.h>




    //satirdaki koordinat sayisinin hesaplanmasi
    int virgulsayisi=0, entersayisi=1;
    for(int i=0; i<strlen(veri); i++)
    {
        if(satirno==entersayisi)
        {
            for(int j=i; veri[j]!='F'; j++)
            {
                if(veri[j]==',')
                    virgulsayisi++;
            }
            break;
        }
        if(veri[i]=='\n')
            entersayisi++;
    }
    //printf("%d. satirdaki koordinat sayisi %d",satirno,virgulsayisi);


    //Satirdaki karakter sayisini bulma
    int Bsayisi=0,say=0;
    for(int i=0; i<strlen(veri); i++)
    {
        if(veri[i]=='B')
        {
            Bsayisi++;
            if(Bsayisi==satirno)
            {
                for(int j=i+1; veri[j]!='F'; j++)
                    say++;
            }
        }
    }

    //Satirin yazdirilmasi
    char satir[say];
    int b=0;
    Bsayisi=0;
    for(int i=0; i<strlen(veri); i++)
    {
        if(veri[i]=='B')
        {
            Bsayisi++;
            if(Bsayisi==satirno)
            {
                for(int j=i+1; veri[j]!='F'; j++)
                    satir[b++]=veri[j];
                satir[b]='\0';
            }
        }
    }
    printf("\nsatir:\n%s\n",satir);


    //Satirdaki koordinatlarin diziye alinmasi
    int dizi[virgulsayisi*2];
    char *ptr=satir;
    b=0;
    while (*ptr)
    {
        if (isdigit(*ptr))
        {
            dizi[b++] = strtol(ptr, &ptr, 10); // Sayıları diziye aktar
        }
        else
        {
            ptr++; // Sayı değilse bir sonraki karaktere geç
        }
    }


    /*for (int i = 0; i < b; i++) {
        printf("%d ", dizi[i]);
    }*/
    for(int i=0; i<b; i++)
        dizi[i]*=15;
    printf("\n");
    /*for (int i = 0; i < b; i++) {
        printf("%d ", dizi[i]);
    }*/



    //Dizide kac adet sekil oldugunu bulma (max 3 olabilir)
    int s1=0,s2=0,s3=0;
    for(int i=2; i<virgulsayisi*2; i+=2)
    {
        if(dizi[0]==dizi[i] && dizi[1]==dizi[i+1])
        {
            i+=2;
            s1=i;
            for(int j=i+2; j<virgulsayisi*2; j+=2)
            {
                if(dizi[i]==dizi[j] && dizi[i+1]==dizi[j+1])
                {
                    j+=2;
                    s2=j;
                    for(int k=j+2; k<virgulsayisi*2; k+=2)
                    {
                        if(dizi[j]==dizi[k] && dizi[j+1]==dizi[k+1])
                        {
                            s3=k+2;
                            break;
                        }
                    }
                }
            }
        }
    }
    //printf("\n %d \n %d \n %d",s1,s2,s3);






    //koordinat düzlemi olusturma
    int ekranx=1300,ekrany=700,bolum=15;
    initwindow(ekranx,ekrany);
    for(int i=0; i<=ekranx; i+=bolum)
    {
        line(i,0,i,ekrany);
    }
    for(int i=0; i<=ekrany; i+=bolum)
    {
        line(0,i,ekranx,i);
    }
    char a[3];
    int x=0,y=0;
    settextstyle(2,HORIZ_DIR,4);
    for(int i=0; i<=ekranx; i+=bolum)
    {
        sprintf(a,"%d",x);
        outtextxy(i,0,a);
        x++;
    }
    for(int i=0; i<=ekrany; i+=bolum)
    {
        sprintf(a,"%d",y);
        outtextxy(0,i,a);
        y++;
    }





    //Sekilleri Cizme ve Alanlarini Hesaplama
    double alan1=0,alan2=0,alan3=0;//sekillerin alanlarini almak icin
    if(s1!=0)
    {
        int sekil1[s1];
        for(int i=0; i<s1; i++)
            sekil1[i]=dizi[i];
        fillpoly(s1/2,sekil1);
        int a1[s1];
        for(int i=0; i<s1; i++)
            a1[i]=sekil1[i]/15;
        for(int i=0; i<=s1/2-3; i+=2)
            alan1+=0.5*fabs((a1[0]-a1[2+i])*(a1[1]-a1[5+i])-(a1[0]-a1[4+i])*(a1[1]-a1[3+i]));
        printf("\n1. Seklin alani: %.2lf",alan1);
        if(s2!=0)
        {
            int sekil2[s2-s1];
            int j=s1;
            for(int i=0; i<s2-s1; i++)
                sekil2[i]=dizi[j++];
            fillpoly((s2-s1)/2,sekil2);
            int a2[s2-s1];
            for(int i=0; i<s2-s1; i++)
                a2[i]=sekil2[i]/15;
            for(int i=0; i<=(s2-s1)/2-3; i+=2)
                alan2+=0.5*fabs((a2[0]-a2[2+i])*(a2[1]-a2[5+i])-(a2[0]-a2[4+i])*(a2[1]-a2[3+i]));
            printf("\n2. Seklin alani: %.2lf",alan2);
            if(s3!=0)
            {
                int sekil3[s3-s2];
                int k=s2;
                for(int i=0; i<s3-s2; i++)
                    sekil3[i]=dizi[k++];
                fillpoly((s3-s2)/2,sekil3);
                int a3[s3-s2];
                for(int i=0; i<s3-s2; i++)
                    a3[i]=sekil3[i]/15;
                for(int i=0; i<=(s3-s2)/2-3; i+=2)
                    alan3+=0.5*fabs((a3[0]-a3[2+i])*(a3[1]-a3[5+i])-(a3[0]-a3[4+i])*(a3[1]-a3[3+i]));
                printf("\n3. Seklin alani: %.2lf",alan3);
            }
        }
    }
    double top=alan1+alan2+alan3;
    printf("\nToplam Sekil Alani: %.2lf\n\nToplam Rezerv Alani(toplam yuzey alani x 10): %.2lf",top,top*10);


    //7 saniye sonra kapat
    delay(7000);
    closegraph();



    //Maliyet bilgilerinin kullanicidan alinmasi
    printf("\n\nBirim Sondaj Maliyetini giriniz(1 ile 10 arasinda olmalidir): ");
    int bsm;
    scanf("%d",&bsm);
    while(bsm<1 || bsm>10)
    {
        printf("Lutfen 1 ile 10 arasinda bir deger giriniz: ");
        scanf("%d",&bsm);
    }
    printf("Birim Platform Maliyetini giriniz: ");
    int bpm;
    scanf("%d",&bpm);




    //koordinat düzlemi olusturma
    ekranx=1300,ekrany=700,bolum=15;
    initwindow(ekranx,ekrany);
    for(int i=0; i<=ekranx; i+=bolum)
    {
        line(i,0,i,ekrany);
    }
    for(int i=0; i<=ekrany; i+=bolum)
    {
        line(0,i,ekranx,i);
    }
    x=0,y=0;
    settextstyle(2,HORIZ_DIR,4);
    for(int i=0; i<=ekranx; i+=bolum)
    {
        sprintf(a,"%d",x);
        outtextxy(i,0,a);
        x++;
    }
    for(int i=0; i<=ekrany; i+=bolum)
    {
        sprintf(a,"%d",y);
        outtextxy(0,i,a);
        y++;
    }



    //Optimum Alan cizimi
    setcolor(5);
    if(s1!=0)
    {
        int sekil1[s1];
        for(int i=0; i<s1; i++)
            sekil1[i]=dizi[i];
        drawpoly(s1/2,sekil1);
        if(s2!=0)
        {
            int sekil2[s2-s1];
            int j=s1;
            for(int i=0; i<s2-s1; i++)
                sekil2[i]=dizi[j++];
            drawpoly((s2-s1)/2,sekil2);
            if(s3!=0)
            {
                int sekil3[s3-s2];
                int k=s2;
                for(int i=0; i<s3-s2; i++)
                    sekil3[i]=dizi[k++];
                drawpoly((s3-s2)/2,sekil3);
            }
        }
    }
    getch();
    closegraph();

    return 0;
}
