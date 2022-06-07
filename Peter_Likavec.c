#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include <ctype.h>

int main(int argc, char *argv[]){
int opt,i = 0,k = 0,j = 0,l = 0,price,count = 1,r = 0;
char warehouse_filter = ' ',item_filter = ' ',print_filter = ' ',tmp = ' ';
char* name,*item_name,*item_name_2,*max_price, *lat, *lon;
double d_lat = 0,d_lon = 0,d,final_d;
char* optstring = ":w:i:n:e:t:p:W";
    while ((opt = getopt(argc, argv, optstring)) != -1) {
    switch (opt)
    {
    case 'w':
        warehouse_filter = 'w';
        name = optarg;
        break;
    case 'i':
        warehouse_filter = 'i';
        item_name = optarg;
        break;
    case 'n':
        if (warehouse_filter == 'e')
            tmp = 'n'; 
        else  
            warehouse_filter = 'n';
        lat = optarg;
        break;
    case 'e':
        if (warehouse_filter == 'n')
            tmp = 'e'; 
        else
            warehouse_filter = 'e';
        lon = optarg;
        break;
    case 't':
        item_filter = 't';
        item_name_2 = optarg;
        break;
    case 'p': 
        item_filter = 'p';
        max_price = optarg;
        break;
    case 'W':
        print_filter = 'W';
        break;
    case ':':
        return 2;
    default:
        return 1;    
    }
}

if ((warehouse_filter == 'e' && tmp == 'n') || (warehouse_filter == 'n' && tmp == 'e')){
    for(size_t i = 0;i<strlen(lat);i++){
        if(isdigit(lat[i]) == 0 && (lat[i]) != '.')
            return 4;
        }
    for(size_t i = 0;i<strlen(lon);i++){    
        if(isdigit(lon[i]) == 0 && (lon[i]) != '.')
            return 4;
        }

    d_lat = atof(lat);
    d_lon = atof(lon);

    if(d_lat > 90 || d_lat < -90)
        return 4;
    if(d_lon > 180 || d_lon < -180)
        return 4;
}
else if ((warehouse_filter == 'e' && tmp == ' ') || (warehouse_filter == 'n' && tmp == ' ')){
    return 3;
}

WAREHOUSE warehouse_new;
WAREHOUSE warehouse_new2;

if (item_filter == 'p')
    price = atoi(max_price);

for(i = 0;i < DB_NUM;i++){
    if(warehouse_filter == 'w'){
        if(strcmp(name,db[i].name) == 0)
            warehouse_new = db[i];
        else  
            continue;
    }       
    else if (warehouse_filter == 'i'){
        j = 0;
            for(k = 0;k<db[i].n;k++){
                if(strcmp(item_name,db[i].items[k].name) == 0)
                    warehouse_new = db[i];
                else{ 
                    j++; 
                continue;
                }
            }       
            if(j == db[i].n)
                continue;
    }
    else if(warehouse_filter == 'n' || warehouse_filter == 'e'){
        GPS gps1 = {d_lat,d_lon};
        d = distance(gps1,db[i].gps);
        if(i == 0){
            final_d = d;
            k = i;
            continue;
        }
        if(final_d > d){
            final_d = d;
            k = i;
        }
        if(i == DB_NUM-1){
            warehouse_new = db[k];
        }
        else{
            continue;
        }
    }
    else{
        warehouse_new = db[i];
    }

    if (item_filter == 't'){
        j = 0;
        for(k = 0;k<warehouse_new.n;k++){
            if (strcmp(item_name_2,warehouse_new.items[k].name) == 0){
                warehouse_new2.n = warehouse_new.n;
                warehouse_new2.gps = warehouse_new.gps;
                strcpy(warehouse_new2.name,warehouse_new.name);
                strcpy(warehouse_new2.items->name,warehouse_new.items[k].name);
                warehouse_new2.items->price = warehouse_new.items[k].price;
                break;
            }
            else{
                j++;
                continue;
            }
        }
    }
    else if(item_filter == 'p'){
        j = 0,r = 0;
        warehouse_new2.n = warehouse_new.n;
        warehouse_new2.gps = warehouse_new.gps;
        strcpy(warehouse_new2.name,warehouse_new.name);
        for(k = 0;k<warehouse_new.n;k++){
            if(warehouse_new.items[k].price <= price){
                warehouse_new2.items[r] = warehouse_new.items[k];
                r++;
                continue;
                }
            else{
                j++;
                continue;
                }
        }
    }
    else{
        warehouse_new2 = warehouse_new;
    }

    if(j == warehouse_new.n && print_filter != 'W')
        continue;
    else if(j == warehouse_new.n)     
        strcpy(warehouse_new2.name,"fail");

    if (item_filter == 't')
        l = 1;
    else if(item_filter == 'p')
        l = r;
    else if(print_filter == ' ' || print_filter != ' ')
        l = warehouse_new.n; 

    if(print_filter == 'W'){
        printf("%s %.3lf %.3lf %d :\n",warehouse_new.name,warehouse_new.gps.lat,warehouse_new.gps.lon,warehouse_new.n);
        if(strcmp(warehouse_new.name,warehouse_new2.name) == 0){
            for(k = 0;k<l;k++){ 
                printf("%d. %s %d\n",k+1,warehouse_new2.items[k].name,warehouse_new2.items[k].price);
            }
        }
    }
    else{
        for(k = 0;k<l;k++){ 
            printf("%d. %s %d : %s %.3lf %.3lf %d\n",count,warehouse_new2.items[k].name,warehouse_new2.items[k].price,warehouse_new2.name,warehouse_new2.gps.lat,warehouse_new2.gps.lon,warehouse_new2.n);
            count++;
        }
    } 
}

return 0;
}