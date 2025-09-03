#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>

#define available 0
#define booked 1
#define max_rooms 30

typedef struct{
    char name[50];
    char phone[15];
}Customer;

typedef struct{
    int room_number;
    int floor;
    char description[30];
    long price;
    int status;
}Room;

 Room rooms[max_rooms];
 int total_rooms=0;

typedef struct{
    int reserve_code;
    Customer cust;
    Room  rm;
    char date[11];
}Booking;

Customer c;

void customer_login(){
    
    printf("%s","Enter your name: ");
    scanf("%s",c.name);
    printf("%s","Enter your phone number: ");
    scanf("%s",c.phone);
    printf("Welcome %s to the Hotel Reservation System\n",c.name);
}

int admin_login(){
    char password[50];
    char correct_pass[50];
    int try=3;
    while(try>0){
        printf("Enter admin password (remaining attempts %d):",try);
        scanf("%s",password);

        FILE *file=fopen("admin_pass.txt","r");
        if (file==NULL){
            puts("Error opening password file!");
            return 0;
        } 
        fscanf(file,"%s",correct_pass);
        fclose(file);
        if(strcmp(password,correct_pass)==0){
            puts("Login successful");
            return 1;// موفق
        } else{
            puts("Wrong password!");
        }
        try--;
    }
    puts("Access blocked");
    return 0;
}

void change_admin_password() {
    char new_pass[50];
    printf("%s","Enter new password: ");
    scanf("%s", new_pass);
    FILE *file=fopen("admin_pass.txt","w");
    fprintf(file,"%s", new_pass);
    fclose(file);
}

void creat_room_file(){
    FILE *prices=fopen("prices.txt","r");
    FILE *rooms_file=fopen("rooms.txt","w");
    if (prices==NULL||rooms_file==NULL){
        puts("Error opening file!");
        return;
    }
    char line[200];
    int floor=0;
    while(fgets(line,sizeof(line),prices)){
        line[strcspn(line, "\n")] = '\0';

    if(strstr(line,"Floor")){
        sscanf(line,"=== Floor %d ===",&floor);
    } else if(strstr(line,"Room")){
        int room_number;
        long price=0;
        char desc[30]="-";
        int status=0;
        sscanf(line,"Room %d",&room_number);
        char *token_pos = strchr(line, ':');
        if (token_pos == NULL) {
            continue;
        }
        
        // حرکت به بعد از دو نقطه
        char *price_start = token_pos + 1;
        
        // حذف فاصله‌های قبل از قیمت
        while (*price_start == ' ') {
            price_start++;
        }
        char price_str[20];
        int i = 0;
        
        // خواندن ارقام تا وقتی که به غیر رقم برسیم
        while (*price_start >= '0' && *price_start <= '9' && i < 19) {
            price_str[i++] = *price_start++;
        }
        price_str[i] = '\0';
        
        // تبدیل به عدد
        
        // استخراج قیمت بدون دستکاری رشته اصلی
        char *end_ptr;
        price = strtol(price_str, &end_ptr, 10);

         if (strstr(line, "Sea View")) {strcpy(desc,"Sea View");  }
         fprintf(rooms_file,"%d|%d|%ld|%d|%s\n", room_number, floor, price, status, desc);
        }
    }
    fclose(prices);
    fclose(rooms_file);
    puts("rooms.txt file created successfully.");
}

void load_rooms(){
    FILE *rooms_file= fopen("rooms.txt", "r");
    if (rooms_file== NULL) {
        printf("Error opening rooms file\n");
        creat_room_file(); // اگر فایل وجود نداشت، اولیش رو بساز
      rooms_file= fopen("rooms.txt", "r"); 
        if (rooms_file== NULL) {
            printf("Error opening file again!\n");
            return;
        }
    }
char line[200];
total_rooms = 0;
while(fgets(line,sizeof(line),rooms_file)&&total_rooms<max_rooms){
    line[strcspn(line, "\n")] = '\0';
    sscanf(line,"%d|%d|%ld|%d|%29[^\n]",&rooms[total_rooms].room_number,&rooms[total_rooms].floor ,&rooms[total_rooms].price,&rooms[total_rooms].status,rooms[total_rooms].description);
total_rooms++;
}
fclose(rooms_file);
}
void update_room_status() {
    FILE *rooms_file= fopen("rooms.txt", "w");
    if (rooms_file== NULL) {
        printf("Error opening rooms file!\n");
        return;
    }
    for (int i = 0; i < total_rooms; i++) {
        fprintf(rooms_file, "%d|%d|%ld|%d|%s\n",rooms[i].room_number,rooms[i].floor,rooms[i].price,rooms[i].status,rooms[i].description);
    }
    fclose(rooms_file);
}

void show_rooms(){
    printf("\nAll Rooms List:\n");
    printf("┌────────┬────────┬────────────────┬─────────┬────────────┐\n");
    printf("│ %-6s │ %-6s │ %-14s │ %-7s │ %-10s │\n", 
           "Floor","Room","Price(Toman)","Status","Description");
    printf("├────────┼────────┼────────────────┼─────────┼────────────┤\n");
    for(int i=0;i<total_rooms;i++){
    printf("│ %-6d │ %-6d │ %-14ld │ %-7s │ %-10s │\n",rooms[i].floor,rooms[i].room_number,rooms[i].price,rooms[i].status==0? "Empty":"Booked",rooms[i].description);
    }
    printf("└────────┴────────┴────────────────┴─────────┴────────────┘\n"); 
}

void show_rooms_with_view(){
    printf("All Rooms with Sea View\n");
    printf("┌────────┬────────┬────────────────┬─────────┐\n");
    printf("│ %-6s │ %-6s │ %-14s │ %-7s │\n" 
           ,"Floor","Room","Price(Toman)","Status");
    printf("├────────┼────────┼────────────────┼─────────┤\n");
    for(int i=0;i<total_rooms;i++){
        if(strstr(rooms[i].description,"ویو دریا")){
            printf("│ %-6d │ %-6d │ %-14ld │ %7s │\n",rooms[i].floor,rooms[i].room_number,rooms[i].price,rooms[i].status==0? "Empty":"Booked");
    }}
    printf("└────────┴────────┴────────────────┴─────────┘\n");
}

void get_current_date(char *date) {
    time_t now=time(NULL);
    struct tm *t=localtime(&now);
    sprintf(date,"%04d/%02d/%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
}

void book_room(){
        Booking new_booking;
        strcpy(new_booking.cust.name, c.name);
        strcpy(new_booking.cust.phone,c.phone);
        
        printf("Enter room number: \n");
        scanf("%d", &new_booking.rm.room_number);
        
        get_current_date(new_booking.date);
        
        new_booking.reserve_code=1000+rand()%9000;
        
       FILE *booking=fopen("booking.txt","a");
       if (booking==NULL){
        puts("Error opening file!");
        return;
    } 
            fprintf(booking,"%d|%s|%s|%d|%s\n",new_booking.reserve_code,new_booking.cust.name, new_booking.cust.phone,new_booking.rm.room_number,new_booking.date);
            fclose(booking);
            for (int i=0;i< total_rooms;i++) {
                if (rooms[i].room_number ==new_booking.rm.room_number) {
                    rooms[i].status=booked;
                    update_room_status();
                    break;
                } }
                printf("Reservation successful. Reservation code: %d\n",new_booking.reserve_code);
}

void show_available_rooms(){
    int found = 0;
    for(int i = 0; i < total_rooms; i++){
        if(rooms[i].status == available){
            found = 1;
            break;
        }
    }
    if(!found) {
        printf("\nNo available rooms.\n");
        return;
    }
    printf("Available Rooms:\n");
    printf("┌────────┬────────┬────────────────┬────────────┐\n");
    printf("│ %-6s │ %-6s │ %-14s │ %-10s │\n", 
           "Floor","Room","Price(Toman)","Description");
    printf("├────────┼────────┼────────────────┼────────────┤\n");
    for(int i = 0; i < total_rooms; i++){
        if(rooms[i].status == available){
            printf("│ %-6d │ %-6d │ %-14ld│ %-10s │\n",rooms[i].floor, rooms[i].room_number,  rooms[i].price, rooms[i].description);
          
        }}
        printf("└────────┴────────┴────────────────┴────────────┘\n");
    printf("Do you want to book a room?\n");
    printf("1.Yes\n2.No\n");
    printf("Choice: ");
    int choice;
    scanf(" %d", &choice);
    if(choice==1) {
        book_room();
    }
    }
void show_available_rooms_with_view(){
        int found = 0;
    for(int i = 0; i < total_rooms; i++){
        if(strstr(rooms[i].description, "Sea View") && rooms[i].status == available){
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\nNo available rooms with Sea View.\n");
        return;
    }
    printf("\nAvailable Rooms with Sea View:\n");
    printf("┌────────┬────────┬────────────────┐\n");
    printf("│ %-6s │ %-6s │ %-14s │\n", 
           "Floor","Room","Price(Toman)");
    printf("├────────┼────────┼────────────────┤\n");
        for(int i=0;i<total_rooms;i++){
            if(strstr(rooms[i].description,"Sea View")&&rooms[i].status==available){
                printf("│ %-6d │ %-6d │ %-14ld│\n",rooms[i].floor,rooms[i].room_number,rooms[i].price);
            }
        }
        printf("└────────┴────────┴────────────────┘\n");
    printf("Do you want to book a room?\n");
    printf("1.Yes\n2.No\n");
    printf("Choice: ");
    int choice;
    scanf(" %d", &choice);
    if(choice==1) {
        book_room();
    }
    }

void cancel_booking(){
    int reserve_code;
    printf("%s","Enter your reservation code: ");
    scanf("%d",&reserve_code);
     FILE *booking=fopen("booking.txt","r");
if(booking==NULL){
    printf("%s","Error opening reservations file");
    return;
}
FILE *temp=fopen("temp.txt","w");
    if(temp==NULL){
        printf("%s","Error");
        return;
    }

     char line[200];
     int found=0;
     int room_number_to_free = 0;
     char name[50], phone[15], date[11];
while(fgets(line,sizeof(line),booking)){
    int code;
        int room_number;
        sscanf(line, "%d|%[^|]|%[^|]|%d|%s", &code, name, phone, &room_number, date);
    
    if(code==reserve_code){
found=1;
room_number_to_free = room_number;

printf("%s","Reservation canceled\n");

    }else{
        fputs(line,temp);
    }
}
fclose(booking);
fclose(temp);
if(found==1){
    remove("booking.txt");
    rename("temp.txt","booking.txt");
    for (int i = 0; i < total_rooms; i++) {
        if (rooms[i].room_number == room_number_to_free) { 
            rooms[i].status = available;
           update_room_status();  break;// تغییر وضعیت اتاق
        }
    }
}
else{
    remove("temp.txt");
    printf("%s","No reservation found with this code\n");
}
}


void show_all_bookings(){
    FILE *booking=fopen("booking.txt","r");
    if (booking==NULL){
        printf("%s","No reservations made");
        return;
    }
    char line[200];
    printf("\nAll Reservations:\n");
    printf("┌──────────┬────────────┬──────────────┬────────┬────────────┐\n");
    printf("│ %-8s │ %-10s │ %-12s │ %-6s │ %-10s │\n", "Code", "Name", "Phone", "Room", "Date");
    printf("├──────────┼────────────┼──────────────┼────────┼────────────┤\n");
    while(fgets(line,sizeof(line),booking)){
        line[strcspn(line, "\n")] = '\0';
        int reserve_code, room_number;
        char date[11], name[50], phone[15];
        
        sscanf(line, "%d|%[^|]|%[^|]|%d|%s", &reserve_code, name, phone, &room_number, date);
        
        printf("│ %-8d │ %-10s │ %-12s │ %-6d │ %-10s │\n", reserve_code, name, phone, room_number, date);
    }
    printf("└──────────┴────────────┴──────────────┴────────┴────────────┘\n");
    fclose(booking);
}

 void filter_booking(){
    int filter_choice;
    printf("%s","Filter reservations by:\n");
    printf("%s","1.Date\n");
    printf("%s","2.Customer name\n");
    printf("%s","3.Reservation code\n");
    printf("Choice: ");
    scanf("%d",&filter_choice);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    char search_term[50];
    printf("Search term: ");
    fgets(search_term, sizeof(search_term), stdin);
    search_term[strcspn(search_term, "\n")] = '\0';
    
    FILE *booking =fopen("booking.txt","r");
    if(booking==NULL) {
        printf("No reservations made\n");
        return;
    }
    
    char line[200];
    printf("\nSearch Results:\n");
    while(fgets(line,sizeof(line),booking)) {
        int reserve_code,room_number;
        char date[11], name[50], phone[15];
        
        sscanf(line,"%d|%[^|]|%[^|]|%d|%s", 
               &reserve_code,name,phone,&room_number,date);
        
        int match =0;
        switch(filter_choice) {
            case 1: 
                if(strstr(date,search_term)!=NULL) match = 1;
                break;
            case 2:
                if(strstr(name,search_term)!=NULL) match = 1;
                break;
            case 3:
            char *endptr;
            long search_code = strtol(search_term,&endptr,10);
            if(*endptr=='\0'&&reserve_code==search_code) {
                match = 1;}
                break;
        }
        if(match) {
            printf("Code: %d | Name: %s | Phone: %s | Room: %d | Date: %s\n",
                reserve_code, name, phone, room_number, date);
        }
    }
    fclose(booking);
}

 void money(){
    int report_type;
    printf("\nIncome Report:\n");
    printf("%s","1.Daily\n");
    printf("%s","2.Weekly\n");
    printf("Choice: ");
    scanf("%d",&report_type);
    load_rooms();
    
    FILE *booking = fopen("booking.txt", "r");
    if (booking == NULL) {
        printf("No reservations made\n");
        return;
    }
    
    char line[200];
    float total_income = 0;
    int reservation_count = 0;
    char current_date[11];
    get_current_date(current_date);
    int current_year, current_month, current_day;
    sscanf(current_date, "%d/%d/%d", &current_year, &current_month, &current_day);
    
    while(fgets(line, sizeof(line), booking)) {
        int reserve_code, room_number;
        char date[11], name[50], phone[15];
        
        sscanf(line, "%d|%[^|]|%[^|]|%d|%s", 
               &reserve_code, name, phone, &room_number, date);
        
        // استخراج تاریخ رزرو
        int year, month, day;
        sscanf(date, "%d/%d/%d", &year, &month, &day);
        float room_price = 0;
for(int i = 0; i < total_rooms; i++) {
    if(rooms[i].room_number == room_number) {
        room_price = (float)rooms[i].price; 
        break;
    }
}
        switch(report_type) {
            case 1:
            printf("Report type: Daily (Today: %s)\n", current_date);
                if(strcmp(date, current_date) == 0) {
                    total_income += room_price;
                    reservation_count++;
                }
                break;
                
  case 2:
  printf("Report type: Weekly\n");
 if(year == current_year && month == current_month) {
    // اختلاف روزها
    int day_difference = current_day - day;
    
    // اگر بین 0 تا 6 روز گذشته
    if(day_difference >= 0 && day_difference < 7) {
        total_income += room_price;
        reservation_count++;
    }
}
break;
}
}
fclose(booking);
printf("\nIncome Report Results:\n");
    printf("Number of reservations: %d\n", reservation_count);
    printf("Total income: %.0f Toman\n", total_income);
    printf("Average income per reservation: %.0f Toman\n", 
        reservation_count > 0 ? total_income / reservation_count : 0);
    }
void show_my_booking(){
FILE *booking=fopen("booking.txt","r");
if(booking==NULL){
    printf("%s","No reservations made\n");
    return;
}
printf("%s","Your Reservations\n");
char line[200];
int found=0;
while(fgets(line,sizeof(line),booking)){
    int reserve_code, room_number;
        char date[11], name[50], phone[15];
        sscanf(line, "%d|%[^|]|%[^|]|%d|%s",
            &reserve_code, name, phone, &room_number, date);
    if(strcmp(name,c.name)==0){
        printf("Reservation code:%d\n",reserve_code);
            printf("Room number: %d\n",room_number);
            printf("Date: %s\n",date);
        found=1;
    }}
    if(!found){
        printf("%s","You have no reservations\n");
    }
fclose(booking);
}

int main() {
    srand(time(NULL)); 
    load_rooms();
        int role;
        while(1){
        printf("%s","\nHotel Reservation System\n");
        printf("%s","1.Admin Login\n");
        printf("%s","2.Customer Login\n");
        printf("%s","3.Exit\n");
        printf("%s","Choice: ");
        scanf("%d",&role);
        
            switch(role) {
                case 1:{ if(admin_login()){
                int admin_choice;
                do{
                        printf("%s","\nAdmin Panel\n");
                        printf("%s","1.Change Password\n");
                        printf("%s","2.View All Bookings\n");
                        printf("%s","3.Search Bookings\n");
                        printf("%s","4.Income Report\n");
                        printf("%s","5.Back\n");
                        printf("%s","Choice: ");
                        scanf("%d", &admin_choice);
                        
                        switch(admin_choice) {
                            case 1: change_admin_password(); break;
                            case 2: show_all_bookings(); break; 
                            case 3: filter_booking(); break;
                            case 4:money(); break;
                            case 5: break;
                            default :
              puts("Invalid choice");
                    } 
                }while(admin_choice!=5);
            }
              break; 
                }
                case 2: {
                customer_login();
                int customer_choice;
                do{
                    printf("%s","\nCustomer Menu\n");
                    printf("%s","1.Book Room\n");
                    printf("%s","2.My Bookings\n");
                    printf("%s","3.Cancel Booking\n");
                    printf("%s","4.Back\n");
                    printf("Choice: ");
                    scanf("%d", &customer_choice);
                    
                    switch(customer_choice) {
                        case 1: {
                            int second_choice;
                            do{
                                printf("%s","\nBook Room\n");
                                printf("%s","1.View Available Rooms\n");
                                printf("%s","2.View Available Rooms with Sea View\n");
                                printf("%s","3.Back\n");
                                printf("Choice: ");
                                scanf("%d",&second_choice);
                                    
                                switch (second_choice)
                                {
                                case 1:
                                    show_available_rooms();
                                    break;
                                case 2: show_available_rooms_with_view(); break;
                                case 3: break; 
                                
                                default:
                                puts("Invalid choice");
                                }
                            }while(second_choice!=3); break; }
                                
                        case 2:show_my_booking(); break;
                        case 3:cancel_booking();break;
                    case 4: break;
                    default:
                                puts("Invalid choice");
                    }
                    }while(customer_choice!=4);
              break; 
            }
              case 3: 
              return 0;
              default :
              puts("Invalid choice");
           }
    }
}


