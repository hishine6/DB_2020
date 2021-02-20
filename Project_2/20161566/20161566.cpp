#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "byqhdznfp97!";
const char* db = "project2";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);	//sql 연결을 시도한다.
	if (connection == NULL)	//연결 실패
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else					//연결 성공
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		FILE* fp = fopen("20161566_1.txt", "r");	//CRUD파일을 연다.
		if (fp == NULL) {
			printf("WRONG CRUD FILE\n");
			mysql_close(connection);
			return 1;
		}
		

		char query[3000];
		while (!feof(fp)) {					//CRUD파일의 query들을 실행시킨다. (create table, insert into)
			fgets(query, sizeof(query), fp);
			int state = mysql_query(connection, query);
			if (state == 1) {
				printf("There is a problem in existing database\n");
			}
		}
		fclose(fp);

		while (1) {	//Select query 반복문
			int type_number=0;
			printf("------- SELECT QUERY TYPES -------\n");
			printf("\t1. TYPE I\n");
			printf("\t2. TYPE II\n");
			printf("\t3. TYPE III\n");
			printf("\t4. TYPE IV\n");
			printf("\t5. TYPE V\n");
			printf("\t0. QUIT\n");
			printf("Which type of query? ");

			scanf("%d", &type_number);
			printf("\n");

			if (!type_number) {	//종료 조건
				printf("HAVE A GOOD DAY :)\n");
				break;
			}

			switch (type_number) {
			case 1:								//query1번 선택
				printf("---- TYPE I ----\n");
				int truck_num;
				while (1) {						//truck 1721이 들어올 때까지 반복
					printf("Input the number of truck : ");
					scanf("%d", &truck_num);
					if (truck_num == 1721 || truck_num==0)
						break;
					printf("Truck %d is not destroyed.\n\n",truck_num);
				}
				if (truck_num == 0)	//0이 입력되었을 경우 type1 query를 탈출
					break;
				while (1) {		//query 1의 반복문
					printf("\n");
					int flag = 0;
					printf("---- Subtypes in TYPE I ----\n");
					printf("\t1. TYPE I-1\n");
					printf("\t2. TYPE I-2\n");
					printf("\t3. TYPE I-3\n");
					printf("Which type of query? ");
					scanf("%d", &flag);
					if (!flag)	//입력이 0일 경우 type 1 query를 탈출
						break;
					int state;
					printf("\n");
					switch (flag) {
					case 1:						//query 1-1
						strcpy(query, "select Customer.Customer_ID, Customer.Customer_name from Customer, Package as E, (Select Package_ID as S from Move   where Transportation_ID='1721'  and Transportation_Device='Truck' and Departure_Time= '2019-06-05 09:00:00' and start_time < '2019-06-05 11:45:00' and end_time is NULL )as T where E.Package_ID = T.S and Customer.Customer_ID = E.Customer_ID;");
						state = mysql_query(connection, query);	//query를 실행시켜 결과를 저장
						printf("-------------- TYPE I-1 ---------------\n");
						printf("** Find all customers who had a package on the truck at the time of the crash. **\n");
						printf("---------------------------------------\n");
						printf("Customer_ID         Customer_name\n");
						printf("---------------------------------------\n");

						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과 출력
						{
							printf("%-20s%-20s\n", sql_row[0], sql_row[1]);
						}
						mysql_free_result(sql_result);
						break;
					case 2:						//query 1-2
						strcpy(query, "select Recipient.Recipient_ID, Recipient.Recipient_name from Recipient, Package as E, (Select Package_ID as S from Move where Transportation_ID='1721' and Transportation_Device='Truck' and Departure_Time= '2019-06-05 09:00:00' and start_time < '2019-06-05 11:45:00' and end_time is NULL )as T where E.Package_ID = T.S and Recipient.Recipient_ID = E.Recipient_ID;");
						state = mysql_query(connection, query);	//queyr를 실행시켜 결과를 저장
						printf("-------------- TYPE I-2 ---------------\n");
						printf("** Find all recipients who had a package on that truck at the time of the crash. **\n");
						printf("---------------------------------------\n");
						printf("Recipient_ID        Recipient_name\n");
						printf("---------------------------------------\n");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과 출력
						{
							printf("%-20s%-20s\n", sql_row[0], sql_row[1]);
						}
						mysql_free_result(sql_result);
						break;
					case 3:						//query 1-3
						strcpy(query, "select Customer.Customer_ID, Customer_name, Recipient.Recipient_ID, Recipient_name, Arrival_Time from Customer, Recipient, Package, (Select Package_ID as S,max(end_time) from Move where Transportation_ID='1721' and Transportation_Device='Truck' and Departure_Time= '2019-06-05 09:00:00' and start_time < '2019-06-05 11:45:00' and end_time is not null )as T where Package.Package_ID = T.S and Customer.Customer_ID = Package.Customer_ID and Recipient.Recipient_ID = Package.Recipient_ID;");
						state = mysql_query(connection, query);	//queyr를 실행시켜 결과를 저장
						printf("---------------------------------------- TYPE I-3 ------------------------------------------\n");
						printf("** Find the last successful delivery by that truck prior to the crash. **\n");
						printf("--------------------------------------------------------------------------------------------\n");
						printf("Customer_ID         Customer_name       Recipient_ID        Recipient_name      Arrival_Time\n");
						printf("--------------------------------------------------------------------------------------------\n");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과 출력
						{
							printf("%-20s%-20s%-20s%-20s%-20s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
						}
						mysql_free_result(sql_result);
						break;
					default:
						printf("Wrong Input, Try Again\n");
					}
					printf("\n");
				}
				break;
			case 2:							//query 2번 선택
				printf("---- TYPE II ----\n");
				while (1) {
					int year=0;
					printf("** Find the customer who has shipped the most packages in certain year **\n");
					printf("Which year? ");
					scanf("%d", &year);
					if (year == 0)	//year가 0이면 query 선택으로 돌아간다.
						break;
					sprintf(query, "with T as (select Customer_ID as t_Customer_ID, count(Customer_ID) as order_num from Package where Year(order_time)=Year('%d-01-01') group by Customer_ID) select Customer_ID, Customer.Customer_name from T, Customer where T.order_num = (select max(order_num) from T) and t_Customer_ID = Customer_ID;", year);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);	//해당 year에 아무런 기록이 없는 경우
					if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
						printf("No Record in the certain year\n\n");
						continue;
					}
					mysql_query(connection, query);					//해당 year에 기록이 있는 경우, query를 돌려서 결과를 저장한다.
					sql_result = mysql_store_result(connection);
					printf("Customer_ID         Customer_name\n");
					printf("---------------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과를 출력
					{
						printf("%-20s%-20s\n", sql_row[0], sql_row[1]);
					}
					mysql_free_result(sql_result);
					printf("\n");
				}
				break;
			case 3:							//query 3번 선택
				printf("---- TYPE III ----\n");
				while (1) {
					int year = 0;
					printf("** Find the customer who has spent the most money on shipping in the past certain year **\n");
					printf("Which year? ");
					scanf("%d", &year);
					if (year == 0)	//year가 0이면 queyr선택으로 돌아간다.
						break;
					sprintf(query, "with T as (select Customer_ID as t_Customer_ID, sum(Cost) as total_Cost from Package, Bills where Year(order_time)=Year('%d-01-01') and Bills.Package_Id = Package.Package_ID group by Customer_ID) select Customer_ID, Customer.Customer_name from T, Customer where T.total_Cost=(select max(total_Cost) from T) and t_Customer_ID = Customer_ID;", year);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);	//해당 year에 아무런 기록이 없는 경우
					if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
						printf("No Record in the certain year\n\n");
						continue;
					}
					mysql_query(connection, query);					//해당 year에 기록이 있는 경우, query를 돌려서 결과를 저장한다.
					sql_result = mysql_store_result(connection);
					printf("Customer_ID         Customer_name\n");
					printf("---------------------------------------\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과를 출력
					{
						printf("%-20s%-20s\n", sql_row[0], sql_row[1]);
					}
					mysql_free_result(sql_result);
					printf("\n");
				}
				break;
			case 4:							//query 4번 선택
				printf("---- TYPE IV ----\n");
				printf("** Find those packages that were not delivered within the promised time **\n");
				strcpy(query, "select Package_ID, Customer.Customer_ID, Customer.Customer_name, Recipient.Recipient_ID, Recipient.Recipient_name, Timeliness from Package, Recipient, Customer where ((Timeliness = 'overnight' and timestampdiff(hour,Order_Time,Arrival_time) > 24) OR (Timeliness = 'second day' and timestampdiff(hour,Order_Time,Arrival_time) > 48)) and customer.customer_id = package.customer_id and recipient.recipient_id = package.recipient_id;");
				mysql_query(connection, query);
				sql_result = mysql_store_result(connection);
				printf("Package_ID          Customer_ID         Customer_name       Recipient_ID        Recipient_name      Timeliness\n");
				printf("----------------------------------------------------------------------------------------------------------------\n");
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//결과를 출력
				{
					printf("%-20s%-20s%-20s%-20s%-20s%-20s", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
				}
				mysql_free_result(sql_result);
				printf("\n");
				break;
			case 5:							//query 5번 선택
				printf("---- TYPE V ----\n");
				char date[40];
				char ID[21];
				char output_filename[40];
				while (1) {
					printf("** Generate the bill for each customer for the past certain month **\n");
					printf("Customer ID: ");
					scanf("%s", ID);	
					if (strcmp(ID, "0") == 0)	//Customer_ID로 0이 입력된 경우 Query Type select 로 돌아간다.
						break;
					
					sprintf(query, "select * from Customer where Customer_ID=%s", ID);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);
					if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {		//ID가 존재하지 않는다면 다시 ID를 입력 받는다.
						printf("There is no such Customer\n\n");
						continue;
					}

					printf("Which month(YYYY-MM)? ");	//년, 월을 입력받는다.
					scanf("%s", date);
					
					sprintf(query, "select * from Package, Bills where Package.package_ID=Bills.package_Id and package.customer_ID='%s'and year(package.order_time) = year('%s-01')and month(package.order_time)=month('%s-01');",ID,date,date);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);
					if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {		//해당하는 년, 월에 record가 존재하지 않는다면 다시 ID를 입력 받는다.
						printf("There is no such Record in that month\n\n");
						continue;
					}

					printf("Generating Bill...\n");
					sprintf(output_filename, "Bill_%s_%s.txt", ID, date);	//outputfile의 이름을 생성한다.

					fp = fopen(output_filename, "w");

					sprintf(query, "select package.Customer_ID, Customer.Customer_name, Customer.Address,sum(bills.cost)as Total_Cost from package, bills, Customer where package.package_id=bills.package_id and package.customer_ID='%s' and year(package.order_time) = year('%s-01') and month(package.order_time)=month('%s-01') and Customer.Customer_ID=package.Customer_ID;", ID, date, date);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);
					fprintf(fp, "-------------------------------------------------Simple Bill------------------------------------------------------\n");
					fprintf(fp, "Customer_ID         Customer_name       Address             Total_cost\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//simple bill 결과를 outputfile에 출력
					{
						fprintf(fp, "%-20s%-20s%-20s%-20s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					mysql_free_result(sql_result);

					fprintf(fp, "\n");
					fprintf(fp, "--------------------------------------------Itemized Billing List-------------------------------------------------\n");
					sprintf(query, "select package.Package_ID, Cost, Prepaid,  Package_type,Timeliness, Hazardous,international from Package, Bills where Package.package_ID=Bills.package_Id and package.customer_ID='%s'and year(package.order_time) = year('%s-01')and month(package.order_time)=month('%s-01');	", ID, date, date);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);
					fprintf(fp, "Package_ID          Cost           Prepaid   Package_type        Timeliness  Hazardous  International\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//itemized bill 결과를 outputfile에 출력
					{
						fprintf(fp, "%-20s%-15s%-10s%-20s%-12s%-11s%s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
					}
					mysql_free_result(sql_result);

					fprintf(fp, "\n");
					fprintf(fp, "------------------------------------------Bill by type of service-----------------------------------------------\n");
					sprintf(query, "select Service, Sum(cost)as Total_cost from Package, Bills, Choose_Service where Package.package_ID=Bills.package_Id and package.customer_ID='%s'and year(package.order_time) = year('%s-01') and month(package.order_time)=month('%s-01') and Package.Package_type=Choose_Service.Package_type and Package.Weight = Choose_Service.Weight and Package.Timeliness = Choose_Service.Timeliness group by Service;", ID, date, date);
					mysql_query(connection, query);
					sql_result = mysql_store_result(connection);
					fprintf(fp, "Service             Total_cost\n");
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)	//bill by service 결과를 outputfile에 출력
					{
						fprintf(fp, "%-20s%-20s\n", sql_row[0], sql_row[1]);
					}
					mysql_free_result(sql_result);
					fclose(fp);
					printf("Generation Completed\n\n");
				}
				break;
			default:
				printf("Wrong Input, Try Again\n");	//0~5 이외의 숫자를 입력한 경우
			}
			printf("\n");
		}

		fp = fopen("20161566_2.txt", "r"); //terminate 파일을 연다.
		if (fp == NULL) {
			printf("WRONG TERMINATE FILE\n");
			mysql_close(connection);
			return 1;
		}
		while (!feof(fp)) {					//drop을 실행한다.
			fgets(query, sizeof(query), fp);
			mysql_query(connection, query);
		}
		fclose(fp);
	}
	return 0;

}
