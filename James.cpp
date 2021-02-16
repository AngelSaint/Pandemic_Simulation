/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


/* The array named person holds all necessary information about each person within the population.

The breakdown of the data is as shown:
First 2 bits holds the status of the person in terms of the pandemic. 0 - Suseptible, 1 - Infected, 2 - Recovered, 3 - Dead.

The next 7 bits holds the age of the person from 0 - 127.
This is the major identification of individuals in terms of events and severity of the disease and is how the array will be ordered.

The next 2 bits holds the severity of the desease on the person.
0 - Asymptomatic, 1 - mild simptoms, 2 - very infectious with mild symptoms, 3 - severely ill and very infectious.
This number will play a part in the probability of death and infections of other people.

The next 2 bits holds the level of health of the person.
0 -  Completely Healthy, 1 - minor complications in health, 2 - minor preexisting conditions (like obesity),
3 - major preexisting conditions (like asthma)
This will aid in the probability of the severity of the desease and the chance of death.

The next 19 bits holds the household array location of the person.
For every person there is a household value that is either shared between other people or by themselves.
This is used for determining spread within a household which will also be randomized based on the household constant.

The next 16 bits holds the occupation array of the person.
For every person they have an occupation whether that be school, daycare, differenmt work occupations, school,
or remote work that is shared with another group.

The last 16 bits of the array contain the location of the social_group array.
For every person, there are a set number of people who they meet with outside of their household or within the household.
These relationships are randomized between households and people individually.

The person will enter the value of the population and the age disparaty of the community.
This will then be used to randomize the values of each person within the array.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>


typedef struct{
    long int status;
    int Infectiontime;
    int Quarantinetime;
    long int house;
    long int work;
    long int social[2];
}People;
 People person[1000000];


 typedef struct {
     People* person_pointer[6];
     int SafetyFactor;
     bool Used;
 }Housing;
 Housing house[500000];


 typedef struct {
     People* person_pointer[20];
     int SafetyFactor;
 }WorkPlaces;
 WorkPlaces work[200000];

 typedef struct {
     People* person_pointer[20];
     int SafetyFactor;
 }SocialGroups;
 SocialGroups social[500000];


 long int household_count = 0;
 long int workplace_count = 0;
 long int Family_Count = 0;
 int Rval = 1000;
 bool Quarantine_Active = false;
 int testchance=0;
 int Quarantine_Depth = 0;

 int main() {
     srand(time(0));

     //Initialization of functions
        


     int age_disparity[6]; //Holds the values for the age disparities.
     long int population_size;    //Holds the value for the population size.


     do {
         printf("Enter the total population within the model. Population size must be less than 1000000\n");
         scanf("%ld", &population_size);
         if (population_size > 1000000) {
             printf("This size is too big\n");
         }
     } while (population_size > 1000000);

     /* The way the age disparity works is well have the user set the demographic of the model. This will aid in determining the rest of the factors within the
     population.

     The demographic is split up like this:
     Child: 0 - 12.
     Teen: 13 - 17.
     Young adult: 18 - 29.
     Adult: 30 - 49.
     Senior: 50 - 69.
     Elderly: 70 - 99
     */



     //Input for the age disparities.
     bool invalid = false;
     do {
         printf("Enter the age disparity of the population in terms of their percentage number. Example enter 20 for 20 percent. The sum of these inputs must equal 100.\n");
         printf("The order you must enter is: 0 - 12, 13 - 17, 18 - 29, 30 - 49, 50 - 69, 70 - 100.\n");
         scanf("%d %d %d %d %d %d", &age_disparity[0], &age_disparity[1], &age_disparity[2], &age_disparity[3], &age_disparity[4], &age_disparity[5]);
         if ((age_disparity[0] + age_disparity[1] + age_disparity[2] + age_disparity[3] + age_disparity[4] + age_disparity[5]) != 100) {
             printf("The sum of the numbers you entered is not equal to 100.\n");
             invalid = true;
         }
     } while (invalid == true);


     printf("Enter the Rvalue\n");
     scanf("%d", &Rval);


     //Calling the generation of people and setting a random infection.
     long int Under_18_age_count;
     long int Under_4_age_count;
     long int Under_Adult_age_count = 0;
     Generate_People(age_disparity, population_size, &Under_18_age_count, &Under_4_age_count, &Under_Adult_age_count);
     GenerateHousing(Under_18_age_count, population_size);
     GenerateWork(population_size, Under_18_age_count, Under_4_age_count);
     printf("WorkSpaces Generated!!!!\n");
     Generate_social(population_size, Under_18_age_count, Under_4_age_count);
     Event_Random_Infection(Under_18_age_count, population_size);
     system("pause");
     long int inf_days[1000];
     inf_days[0] = 1; //Setting the count for the first day of infections to be 1 for the random person who gets infected.
     long int dead_days[1000];
     dead_days[0] = 0;
     int current_day = 1;//Starting at day 1
     int ret;
     bool week =false;
     int weekstart = 0;


        FILE *Output = fopen("Output1.txt", "w");
        do {

            Event_Go_To_Work();
            Event_Go_Home();
            Event_See_Family();
            DayPasses(population_size);
            inf_days[current_day] = Count_Infected(population_size);
            dead_days[current_day] = Count_Dead(population_size);
            fprintf(Output, "%ld %ld %ld %ld %ld % d % d\n", current_day, inf_days[current_day]- inf_days[current_day-1], dead_days[current_day]- dead_days[current_day-1],Count_Infected(population_size), Count_Dead(population_size), testchance, Quarantine_Depth);
            if (week == true && current_day < weekstart +6) ret =1;
            else ret = Prompt(inf_days, population_size, current_day);

            if (ret == 3) {
                Quarantine_Active = true;
                printf("Enter the chance in percent that someone gets tested\n");
                scanf(" %d", &testchance);
                printf("Enter the Quarantine level. 0 for only the person. 1 for the whole household as well. 2 for the entire workplace as well. 3 for the entire social group as well. ");
                scanf(" %d", &Quarantine_Depth);
        
                ret = 1;
            }
            if (ret == 4) {
                int partynum = 0;
                do {
                    
                    printf("Enter the number of parties that will occur (less than %d):\n", (Under_Adult_age_count / 200));
                    scanf(" %d", &partynum);
                    if (partynum < (Under_Adult_age_count / 200)) {
                        for (int p = 0; p < partynum; p++) {
                            Event_Party(Under_18_age_count, Under_Adult_age_count);
                        }
                        for (long int c = Under_18_age_count; c < Under_Adult_age_count; c++) {//Clearing attendance
                            if ((person[c].status & 0x00000004) == 0x00000004) {
                                person[c].status = person[c].status ^ 0x00000004;
                            }
                        }
                        ret = 1;
                        goto enddo;
                    }
                    else {
                        printf("Thats too many\n");
                        continue;
                    }
                } while (partynum >= (Under_Adult_age_count / 200));
            enddo:;
            }
         if (ret == 1) {
             current_day++;
             printf("\n\n\n*******************NEW DAY %d************************\n\n\n", current_day);
         }
         else if (ret == 2) {
             current_day++;
             week = true;
             weekstart = current_day;
         }
         else if (ret == 0) goto endall;
         else goto endall;

     } while (ret != 'q');
        
 endall:;
     fclose(Output);

     return 0;
 }




 void Print_People_Div(int population_size) {
     //Outputting the values of people based on a range.
     //I want to print 100 people based on the population size.
     //Take population size and divide it by 100. Use that value to multiply the 
 //INFECTED:% d  DEAD ? % d  RECOVERED ? % d
     int population_division = population_size / 100;
     for (int i = 0; i <= 100; i++) {
         printf("********************************\n");
         printf("Person %d   Status:  0x%X   :::::: Age: %d ::::::  \n", (i * population_division), person[i * population_division].status, ((person[i* population_division].status & 0x3F800000) >> 23));
         printf("Person %d   House:  0x%lX   ", (i * population_division), person[i * population_division].house);

         if (person[i * population_division].house != -1)
             printf(":::::: House: %d \n", person[i * population_division].house);
         else
             printf("\n");
         printf("Person %d   Work:  0x%lX   ", (i * population_division), person[i * population_division].work);

         if (person[i * population_division].work != -1)
             printf(":::::: Work: %d \n", person[i * population_division].work);
         else
             printf("\n");
         printf("Person %d   Family:  0x%lX \n", (i * population_division), person[i * population_division].social[0]);
         if (person[i * population_division].social[0] != -1)
             printf(":::::: Family: %d        ", person[i * population_division].social[0]);
         else
             printf("\n");

         printf("********************************\n\n");
     }
 }




    void Print_Everyone(int population_size) {
        for (int i = 0; i <= population_size; i++) {
            printf("********************************\n");
            printf("Person %d   Status:  0x%X   :::::: Age: %d //INFECTED:% d       DEAD ? % d      RECOVERED ? % d \n", (i ), person[i  ].status, ((person[i].status & 0x3F800000) >> 23), ((person[i].status & 0x80000000) >> 31), ((person[i].status & 0x40000000) >> 30), ((person[i].status & 0x00000001)));
            printf("Person %d   House:  0x%lX   ", (i  ), person[i  ].house);
            if (person[i  ].house != -1)
                printf(":::::: House: %d \n", person[i  ].house);
            else
                printf("\n");
            printf("Person %d   Work:  0x%lX   ", (i  ), person[i  ].work);

            if (person[i].work != -1)
                printf(":::::: Work: %d \n", person[i  ].work);
            else
                printf("\n");

            printf("Person %d   Family:  0x%lX   ", (i  ), person[i].social[0]);
            if (person[i].work != -1)
                printf(":::::: Family: %d \n", person[i].social[0]);
            else
                printf("\n");

            printf("********************************\n\n");
        }
    

   
}

/* Within Generate_People I will first set the ages for everyone within the array.
This will be done by multiplying the population size by the percentage for each disparity.
This number is then divided once again for each individial age.

When dividing the population_size by the disparity there will be a remainder at the end.
This remainder is found by performing the calculations for each count. I then subtract the total
population count by the sum of each age count to see if there is a remainder.
When there is a remainder it will be added to the children_count, to account for perhaps a newborn
who was born during the pandemic. From calculations in excel this number is always small at maximum being
5 since that is the maximum number that can be made from all the decimals from each age group.

The same is done for each individual age count such that the youngest recieves the remainder from each
count.


*/

    void Generate_People(int* age_disparity, long  int population_size, long int* Underage_Count, long int* Under_4_age_count, long int* Under_Adult_age_count) {

    long int age_count_no_remainder[6] = { 0,0,0,0,0,0 };
    unsigned int population_count_no_remainder = 0;

    // Calculating the counts for each age group.

    unsigned int age_count[6];

    for (int a = 0; a <= 5; a++) {
        age_count[a] = 0;
        age_count[a] = population_size * age_disparity[a] * 0.01;
        population_count_no_remainder += age_count[a];
    }
    age_count[0] += (population_size - population_count_no_remainder);
    int totalage = 0;

    *Underage_Count = age_count[0] + age_count[1];
    *Under_Adult_age_count = age_count[2];
    //Calculating the counts for each individual age.
    long unsigned int individual_age_count[101];

    for (int b = 0; b <= 12; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[0] / 13;
        age_count_no_remainder[0] += individual_age_count[b];
    }
    individual_age_count[0] += (age_count[0] - age_count_no_remainder[0]);

    *Under_4_age_count = individual_age_count[0] + individual_age_count[1] + individual_age_count[2] + individual_age_count[3];


    for (int b = 13; b <= 17; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[1] / 5;
        age_count_no_remainder[1] += individual_age_count[b];
    }
    individual_age_count[13] += (age_count[1] - age_count_no_remainder[1]);

    for (int b = 18; b <= 29; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[2] /12;
        age_count_no_remainder[2] += individual_age_count[b];
    }
    individual_age_count[18] += (age_count[2] - age_count_no_remainder[2]);


    for (int b = 30; b <= 49; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[3] / 20;
        age_count_no_remainder[3] += individual_age_count[b];
    }
    individual_age_count[30] += (age_count[3] - age_count_no_remainder[3]);

    for (int b = 50; b <= 69; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[4] /20 ;
        age_count_no_remainder[4] += individual_age_count[b];
    }
    individual_age_count[50] += (age_count[4] - age_count_no_remainder[4]);

    for (int b = 70; b < 100; b++) {
        individual_age_count[b] = 0;
        individual_age_count[b] = age_count[5] /31;
        age_count_no_remainder[5] += individual_age_count[b];
    }
    individual_age_count[99] += (age_count[5] - age_count_no_remainder[5]);

    individual_age_count[100] = 0;

    //Checking the count for the individual age totals.
    /*
    int age_total = 0;
    for (int i = 0; i <= 100; i++) {
        age_total += individual_age_count[i];

    }
    printf("Total count is %d\n", age_total); 
    */




    //Assigning the age for each person.
    //We can make this portion more efficient
    //Were assigning values to each person based on the age.
    //So we assign each person until individual_age_count is reached
    //Then we add individual_age_count the starting point
    //Starting point, Ending point.

    unsigned long int age = 0;
    long int starting_point = 0;
    long int s;
    long int ending_point = individual_age_count[0];
    for (age = 0; age < 100; age++) {
        for (s = starting_point; s < ending_point; s++) {
            person[s].status = 0;
            person[s].house = -1;
            person[s].work = -1;
            person[s].social[0] = -1;
            person[s].social[1] = -1;
            person[s].status |=  (age << 23);
            person[s].Infectiontime=0;
            person[s].Quarantinetime = 0;
            //printf("Age being used is: %d, Current Starting_Point is: %d, Current ending_point is :%d\n", age, s, ending_point);
        }
        //printf("Person %d has %d bit shifted as 0x%X. Their final value for status is 0x%X\n", starting_point, age, (age << 7), person[starting_point].status);
        starting_point += individual_age_count[age];
        ending_point += individual_age_count[age + 1];
    }
    return;
}

/*
Generate Houses:
There are in total going to be 6 types of housings based on the number of tenants.
Each house will hold pointers for their tenants 6 pointers per struct.
Each house will also have a safety factor which will be a regular int.
That’s it for the struct.

In terms of the organization of housings I will keep them constant.
Based on the population we will have to assign the number of homes based on this ratio:
30% are single
15% are double
10% are triple
20% are quadruple
15% are 5 bedrooms
10 % are 6 bedrooms.
The number of houses will have to match the population like this:
We will have to group those who are below 18 into groups
1 -> 20%
2 -> 50%
3 -> 25%
4 -> 5%
For every group below 18 there are 1.8 people above 1.8 assigned to them.

The rest of the people can be assigned wherever


*/
void GenerateHousing(long int Underage_Count,long int population_size) {

    for (long int k = 0; k < 100000; k++) { //Initializing the houses.
        for (int f = 0; f < 6; f++) {
            house[k].person_pointer[f] = 0;
        }
        house[k].Used = false;
    }
    


    long int Over_Age_Count = population_size - Underage_Count;
    long int group_count[5];
    int group_count_no_remainder = 0;
    //Assigning the group counts for those under 18.
    group_count[0] = (Underage_Count * 20) / 215;
    group_count[1] = (Underage_Count * 50) / 215;
    group_count[2] = (Underage_Count * 25) / 215;
    group_count[3] = (Underage_Count * 4) / 215;
    group_count[4] = 0;

    //In the case that there is a remainder in the calcuation to ensure that noone is homeless we find the difference between what the full ceiling value
    //should be and add it to whichever group it corresp
    group_count_no_remainder = group_count[0] * 1 + group_count[1] * 2 + group_count[2] * 3 + group_count[3] * 4;
    int remainder = Underage_Count - group_count_no_remainder;
    while ((remainder - 2) != -1 && (remainder - 2) > 0) {
        group_count[1]++;
        remainder -= 2;
    }
    if (remainder = 1) {
        group_count[0]++;
        remainder = 0;
    }
    //Here we have a loop that goes through all the groups and assigns the pointers to the people to the array
    //To find the group we select from a random number wihtin the range of Underage people.
    //If the person already has a house assigned then we skip that person. 
    //If the person does not have a house then we assign them to the group and assign the house based on i.
    //We also assign the pointer to the person to the house array adn determine the safety factor of these houses randomly.

    long int s;
    long int starting_point = 0;
    long int ending_point = group_count[0];
    double completion;
    double denom;
    for (int group = 0; group < 4; group++) {

        long int random_person = 0;


        int x = 0;
        for (s = starting_point; s < ending_point; s++) {

            house[s].SafetyFactor = rand() % 7 + 1;

            int person_count = 0;
            //Setting the children into the home.
            if (group == 3) {
                for (int k = 0; k < Underage_Count; k++) { //Once we reach the groups, to make things faster, we assign
                    if (person_count == 4) {
                        break;
                    }
                    if ((person[k].house != -1)) {
                        continue;
                    }
                    else {
                        person[k].house = s;
                        house[s].person_pointer[person_count] = person + k;
                        person_count++;
                    }
                } 
            }
            else {
                do {

                    random_person = (rand() * rand()) % (Underage_Count + 1);
                    //printf("PAST RAND 2. Group Number = %ld out of %ld ||||| Person Count = %d out of %d \n", s, group_count[0] + group_count[1] + group_count[2], person_count + 1, group + 1);
                    denom = (group_count[0] + group_count[1] + group_count[2]);
                    completion = ((s+1) / denom) * 100;
                    printf("Household Generation Completion %f%%\n", completion);

                    if ((person[random_person].house != -1)) {
                        continue;
                    }
                    else {
                        person[random_person].house = s;
                        house[s].person_pointer[person_count] = person + random_person;
                        person_count++;
                    }
                } while (person_count < (group+1));
            }
         //Finding the adult to pair.
            int parent_chance = (rand() % (101));

            if (parent_chance > 90)
                x = 1;
            else
                x = 2;
            do {
                random_person = ((rand() * rand()) % (population_size - Underage_Count + 1)) + Underage_Count;
                if ((person[random_person].house != -1)) {
                    continue;
                }
                else {
                    person[random_person].house = s;
                    house[s].person_pointer[person_count] = person + random_person;
                    person_count++;
                }

            } while (person_count < (group + x + 1));
        }
        starting_point += group_count[group];
        ending_point += group_count[group + 1];
    }

    //Were now going to randomly assign the rest of the houses.
    int  Remaining_count = 0;
   
        for (int i = Underage_Count; i < population_size; i++){
            if (person[i].house == -1) {
                Remaining_count++;
            }
        }
        //printf("Underage Count: %d Remaining Count: %d\n", Underage_Count, Remaining_count);
        starting_point = group_count[0]+ group_count[1]+group_count[2]+ group_count[3];
    group_count[0] = (Remaining_count * 20) / 215;
    group_count[1] = (Remaining_count * 50) / 215;
    group_count[2] = (Remaining_count * 25) / 215;
    group_count[3] = (Remaining_count * 4) / 215;
    group_count[4] = 0;
    //In the case that there is a remainder in the calcuation to ensure that noone is homeless we find the difference between what the full ceiling value
    //should be and add it to whichever group it corresp
    group_count_no_remainder = group_count[0] * 1 + group_count[1] * 2 + group_count[2] * 3 + group_count[3] * 4;
 remainder = Remaining_count - group_count_no_remainder;
    //printf("Remainder is: %d , Remaining Count is: %d, Groups 1 through 4 are %d || %d || %d || %d || which equals %d\n", remainder, Remaining_count,group_count[0], group_count[1], group_count[2], group_count[3], group_count[0]*1+ group_count[1]*2+group_count[2]*3+ group_count[3]*4);
    while ((remainder - 2) != -1 && (remainder - 2) > 0) {
        group_count[1]++;
        remainder -= 2;
    }
    if (remainder = 1) {
        group_count[0]++;
        remainder = 0;
    }

    ending_point = starting_point + group_count[0];
    long int y;
    long int Starting_search = Underage_Count;
    for (int group = 0; group < 4; group++) {
        for (s = starting_point; s < ending_point; s++) {
            house[s].SafetyFactor = rand() % 7 + 1;
            int person_count = 0;
            //Setting the people into the houses.
            do {
                //printf("PAST RAND 2. Group Number = %d out of %d ||||| Person Count = %d out of %d \n", s, group_count[0] + group_count[1] + group_count[2] + group_count[3] + group_count[4], person_count + 1, group + 1);
                for (y = Starting_search; y < population_size; y++) {
                    if ((person[y].house != -1)) {
                        continue;
                    }
                    else {
                        person[y].house = s;
                        house[s].person_pointer[person_count] = person + y;
                        person_count++;
                        Starting_search = y;
                        break;
                    }
                    if (y == (population_size - 1)) {
                        break;
                    }
                }
            } while (person_count < (group + 1));
        }
        starting_point += group_count[group];
        ending_point += group_count[group + 1];
    }
    int person_count2 = 0;
    // If there are any people without a house at this point we add it here.
    house[ending_point].SafetyFactor = rand() % 7 + 1;
    for (long int k = 0; k < population_size; k++) {
        if (person[k].house == -1) {
            person[k].house = ending_point;
            house[ending_point].person_pointer[person_count2];
            person_count2++;
        }
    }
    household_count = ending_point;
    return;


}
/*
Generate Work:

For those under the age of 18, They go to to a workplace that is like a school.
We will group them in 20 groups for each class. Each class will also be accompanied by a teacher which will be chosen from the adults randomly like in the last function.

Each adult will be assigned to a different job.
Each workplace will just be group by age here in order of age.

In the future I will change this such that it is more based around the different ages to be more diverse.
Each job gorup will have 15-16 people.

*/



    //For every child under the age of 4, we put them in a group with either a parent in the same house or a babysitter from a random place.

    void GenerateWork(long int population_size, long int Underage_Count,long  int Under_4_age_Count) {
        
        int workcount[200000];
        int person_count = 0;
        long int workspace = 0;
        for (long int clear = 0; clear < 200000; clear++) {
            workcount[clear] = 0;
            for (int p = 0; p < 20; p++) {
                work[clear].person_pointer[p] = 0;
            }
        }//Initializing the count for all the workspaces to be 0;
        
 
            for (long int c = 0; c <= Under_4_age_Count; c++) {
                   //First we check if the current child has a sibling around the same age that has already been defined.
                for (long int o = 0; o < c; o++) {
                    if (person[c].work == person[o].work && person[o].work != -1) {
                        //printf("Current child: %d, Other child is: %d Their work is %d\n", c, o, person[o].work);
                        workcount[person[o].work]++;
                        person[c].work = person[o].work;
                        work[workspace].person_pointer[workcount[person[o].work]] = person + c;
                        goto endc;
                    }
                }


                // We check if the household has more than one adult.
                int adult_count = 0;
                People* adult_pointer;
                for (long int h = 0; h < 6; h++){
                    if (house[person[c].house].person_pointer[h] != 0 ){
                    if ((house[person[c].house].person_pointer[h]->status >> 23) > 17) {
                        //printf(" Child: %d House: %d Tenant: %d, Age: %d\n",c, person[c].house,h, (house[person[c].house].person_pointer[h]->status >> 23));
                        adult_count++;
                        if (adult_count == 2) {
                            adult_pointer = house[person[c].house].person_pointer[h];
                            break;
                        }
                    }
                    }
                }
                if (adult_count < 2) {
                   
                    int found_nanny = 0;
                    do {
                        long int random_person;
                        random_person = ((rand() * rand()) % (population_size - Underage_Count)) + Underage_Count;
                        if ((person[random_person].work != -1) && (person[random_person].house != person[c].house)) {
                            work[workspace].person_pointer[workcount[workspace]] = person + random_person;// Setting the first person to be the parent
                            person[random_person].work = workspace;

                            workcount[workspace]++;
                            work[workspace].person_pointer[workcount[workspace]] = person + c; //Setting the next person to be the child.
                            person[c].work = workspace;
                            //printf("Child %d has work %d with Nanny %d which is also %d", c, person[c].work, random_person, person[random_person].work);
                            found_nanny++;
                            work[workspace].SafetyFactor = rand() % 7 + 1;
                            workspace++;
                            goto endc;
                        }
                    } while (found_nanny < 1);
                }
                else {

                    work[workspace].person_pointer[workcount[workspace]] = adult_pointer;// Setting the first person to be the parent
                    adult_pointer->work = workspace;
                    workcount[workspace]++;
                    work[workspace].person_pointer[workcount[workspace]] = person + c; //Setting the next person to be the child.
                    person[c].work = workspace;
                    work[workspace].SafetyFactor = rand() % 7 + 1;
                    workspace++;

                }
            endc:;
            }



    //Setting the jobs for school for those under 18.
    long int start = Under_4_age_Count+1;
    long int counter = 0;
                do {
                    for (start; start < Underage_Count; start++) {
                        if (counter > 14) { break;}
                        else if (start + counter == Underage_Count) { break; }

                        work[workspace].person_pointer[counter] = person + start;
                        person[start].work = workspace;
                        counter++;
                    }
                   

                    int teacher_found = 0;
                    do {//Finding a teacher for the class.
                    People* adult_pointer = person + ((rand() * rand()) % (population_size - Underage_Count + 1)) + Underage_Count;
                        if (adult_pointer->work != -1) {
                            work[workspace].person_pointer[counter] = adult_pointer;// Setting the first person to be the parent
                            adult_pointer->work = workspace;
                            teacher_found++;
                            break;
                        }
                    } while (teacher_found < 1);
                    work[workspace].SafetyFactor = rand() % 7 + 1;
                    workspace++;;
                    counter = 0;
                } while (start < Underage_Count);
                



                start = Underage_Count;
                do {
                    for (start; start < population_size; start++) {
                        if (counter > 14) {
                            break;
                        }
                        if (person[start].work == -1) {
                            work[workspace].person_pointer[counter] = person + start;
                            person[start].work = workspace;
                            counter++;
                        }
                        else continue;
                    }
                    work[workspace].SafetyFactor = rand() % 7 +1;
                    workspace++;
                    counter = 0;
                } while (start < population_size);
                workplace_count = workspace;

                return;
}

/*
Social Group function:

Those who are under 4 obtain the same social group as their parents in pass 1. 

Those from 4-17 get their own social Groups.

Those from 18-99 get 2 social groups. 1 for family and 1 for themselves. 

Each person age 4+ should have 2 social groups like this.

For the first pass we first assign all the adults to the family social group by grouping households.
each family group will have 3 households.

For the second pass we go through individual age groups above 4 and randomly assign 10 people.

*/
void Generate_social(long int population_size,long int Under_4_Age_Count, long int Under_Age_Count) {
    
    long int House_count = 0; //For the check to see if all houses are used.
    long int Social_Count= 0;
    long int StartSearch = 0;
    long int b = 0;
    long int End_Search;
    double completion1;
    double completion2;
    double denom;
    do{
        int Person_Counter = 0;
        for (int h = 0; h < 3; h++) {
            House_count = 0;
            if (Social_Count > household_count*0.5/3)
            {
                if (Social_Count > household_count * 0.9 / 3) {
                    End_Search = household_count;
                }
                else End_Search = Social_Count * 3;

            for (b = StartSearch; b <= End_Search; b++) {//This is a quick check to see if all houses are used in case there are no more houses left.
                if (house[b].Used == true) {
                    House_count++;
                }
            }
            //printf("House Count: %ld out of %ld\n", House_count+ StartSearch, household_count);
            denom = household_count;
            completion1 = ((House_count + StartSearch) / denom) * 100;
            //printf("Social Generation Completion: %f%% \n", completion1);
            }
            if (House_count+ StartSearch > household_count) {
                
                Social_Count++;//If we are out of houses we should increase the Social_Count and leave the current one.
                goto endloops;
            }
            else if (Social_Count > household_count * 0.5 / 3) {//This is also used to make the calculations faster since when 75% of all households are used, the amount of failed random tests becomes too much.
                for (StartSearch; StartSearch <= household_count+1; StartSearch++) {
                    if (house[StartSearch].Used == false) {
                        house[StartSearch].Used = true;
                        for (int c = 0; c < 6; c++) {
                            if (house[StartSearch].person_pointer[c] != 0) {
                                social[Social_Count].person_pointer[Person_Counter] = house[StartSearch].person_pointer[c];
                                house[StartSearch].person_pointer[c]->social[0] = Social_Count;
                                //printf("Person %d got Social %d \n", (house[rand_house].person_pointer[c] - person), Social_Count);
                                Person_Counter++;
                            }
                        }
                        goto endhouses;
                    }
                    else continue;
                }
            }

            long int rand_house; 
            do {        //This is the part where we assign the houses.
                rand_house = (rand() * rand()) %(household_count + 1);
                //printf("Random House : %ld, Social Group: %ld out of apx. %ld\n", rand_house,Social_Count,household_count/3);
                denom = (household_count / 1.5);
                completion2 = (Social_Count / denom ) * 100;
                //printf("Social Generation Completion: %f%%\n", completion2);
                    if (house[rand_house].Used == false) {
                        house[rand_house].Used = true;
                        for (int c = 0; c < 6; c++) {
                            if (house[rand_house].person_pointer[c] != 0) {
                                social[Social_Count].person_pointer[Person_Counter] = house[rand_house].person_pointer[c];
                                house[rand_house].person_pointer[c]->social[0] = Social_Count;
                                //printf("Person %d got Social %d \n", (house[rand_house].person_pointer[c] - person), Social_Count);
                                Person_Counter++;
                            }
                        }
                        goto endhouses;
                    }
                    else {
                        //printf("::::::::::::::::::::::::FAILED RAND\n");
                        continue;
                    }
            } while (house[rand_house].Used != false);

        endhouses:;
        }
        social[Social_Count].SafetyFactor = rand() % 7 +1;
        Social_Count++;
    } while (1);
    endloops:;
    Family_Count = Social_Count;


    return;

}






void Event_Random_Infection(long int Underage_Count, long int population_size) {
    People* Random_Person;
    do {
         Random_Person = person + (rand() * rand()) % (population_size - Underage_Count ) + Underage_Count;
         //printf("Person %ld Status is: %lX, thing is %lX\n", Random_Person - person, Random_Person->status, Random_Person->status & 0x80000000);
        if ((Random_Person->status & 0x80000000) == 0x00000000) {
            Random_Person->status = Random_Person->status | 0x80000000; // Setting the status of the person to infected.
            goto endinfection;
        }
    } while (1);
endinfection:printf("Person infected is %ld, their age was %ld going to work %ld, Their infection status is %lX\n", Random_Person - person, (Random_Person->status & 0x3F800000)>>23,Random_Person->work, Random_Person->status & 0x80000000);
    return;
}


void Event_Go_To_Work() {


  
    int infected_count;
    for (long int i = 0; i <= workplace_count; i++) {
        //printf("Made it here in go to work function current work is : %d\n", i);
        infected_count = 0;
        for (int p = 0; p < 20; p++) {
            if (work[i].person_pointer[p] != 0) {
                //printf("       Checking person %ld , Their current infection status is: %lX, life status is %lX\n", work[i].person_pointer[p] - person, work[i].person_pointer[p]->status & 0x80000000, work[i].person_pointer[p]->status & 0x40000000);
                if ((work[i].person_pointer[p]->status & 0x80000000) == 0x80000000 && (work[i].person_pointer[p]->status & 0x00000002) != 0x00000002) {
                    infected_count++;
                    //printf("::::::::::::::::::::::::::::::::::::::::::Person infected is %ld they are currently infected at work %ld\n", work[i].person_pointer[p] - person, i);
                }
            }
        }
        for (int z = 0; z < 20; z++) {
            if (work[i].person_pointer[z] != 0) {
                if (((work[i].person_pointer[z]->status & 0x80000000) != 0x80000000) && ((work[i].person_pointer[z]->status & 0x00000001) != 0x00000001)) {
                    int Random_Chance = rand() % 10000;
                    double chance = (infected_count / (float)5) * (Rval / (float)social[i].SafetyFactor);
                    if (Random_Chance < chance) {//This is how I will determine the chance of infection
                        work[i].person_pointer[z]->status = work[i].person_pointer[z]->status | 0x80000000;
                        printf("Person %ld got infected at work %ld\n", work[i].person_pointer[z] - person, i);
                    }
                }
            }
        }
    }
    return;
}

void Event_Go_Home() {
    int infected_count;
    for (long int i = 0; i < household_count; i++) {
        infected_count = 0;
        for (int p = 0; p < 6; p++) {
            if (house[i].person_pointer[p] != 0) {
                if ((house[i].person_pointer[p]->status & 0x80000000) == 0x80000000) {//Also Checking if the person is alive.
                    infected_count++;
                }
            }
            for (int z = 0; z < 6; z++) {
                if (house[i].person_pointer[z] != 0) {
                    if (((house[i].person_pointer[z]->status & 0x80000000) != 0x80000000) && ((house[i].person_pointer[z]->status & 0x00000001) != 0x00000001)) {
                        int Random_Chance = rand() % 10000;
                        double chance = (infected_count / (float)2) * (Rval / (float)social[i].SafetyFactor);
                    if (Random_Chance < chance) {//This is how I will determine the chance of infection
    house[i].person_pointer[z]->status = house[i].person_pointer[z]->status | 0x80000000;
    printf("Person %ld got infected at house  %ld\n", house[i].person_pointer[z] - person, i);
}
                    }
                }
            }

        }

    }
    return;
}
void Event_See_Family() {
    int infected_count;
    for (long int i = 0; i < Family_Count; i++) {
        infected_count = 0;
        int randnum = rand() % 100;
        if (randnum < 20) {// Which family meets is random.
            for (int p = 0; p < 6; p++) {
                if (social[i].person_pointer[p] != 0) {
                    if ((social[i].person_pointer[p]->status & 0x80000000) == 0x80000000 && (social[i].person_pointer[p]->status & 0x00000002) != 0x00000002) {//Also Checking if the person is alive or quarantined.
                        infected_count++;
                    }
                }
                for (int z = 0; z < 6; z++) {
                    if (social[i].person_pointer[z] != 0) {
                        if (((social[i].person_pointer[z]->status & 0x80000000) != 0x80000000) && ((social[i].person_pointer[z]->status & 0x00000001) != 0x00000001)) {
                            int Random_Chance = rand() % 10000;
                            double chance = (infected_count / (float)5) * (Rval / (float)social[i].SafetyFactor);
                            if (Random_Chance < chance) {//This is how I will determine the chance of infection
                                social[i].person_pointer[z]->status = social[i].person_pointer[z]->status | 0x80000000;
                                printf("Person %ld got infected at family  %ld\n", social[i].person_pointer[z] - person, i);
                            }
                        }
                    }
                }

            }
        }
    }
    return;
}

void DayPasses(long int population_size) {
    int DeathChance = 0;
    for (long int i = 0; i < population_size; i++) {
        if ((person[i].status & 0x80000000) == 0x80000000) {
            person[i].Infectiontime++;
            if (Quarantine_Active == true) {
                int TestProb = rand() % 100;
                if (TestProb < testchance && person[i].Infectiontime> 2) {//Greater than 2 assuming that they only test positive after 2 days.
                    person[i].status = person[i].status ;
                    if (Quarantine_Depth > 0) {
                        for (int p = 0; p < 6; p ++ ) {
                            if (house[person[i].house].person_pointer[p] != 0)
                                house[person[i].house].person_pointer[p]->status = house[person[i].house].person_pointer[p]->status | 0x00000002; //Everyone in the same house is quarantined.
                        }
                    }
                    else if (Quarantine_Depth > 1) {
                        for (int p = 0; p < 20; p ++ ) {
                            if (work[person[i].work].person_pointer[p] != 0)
                                work[person[i].work].person_pointer[p]->status = work[person[i].work].person_pointer[p]->status | 0x00000002; //Everyone in the same work is quarantined.
                        }
                    }
                    else if (Quarantine_Depth > 2) {
                        for (int p = 0; p < 20; p++) {
                            if (social[person[i].social[0]].person_pointer[p] != 0)
                                social[person[i].social[0]].person_pointer[p]->status = social[person[i].social[0]].person_pointer[p]->status | 0x00000002; //Everyone in the same social circle is quarantined.
                        }
                    }
                }
            }
            if ((person[i].status & 0x00000002) == 0x00000002) {
                person[i].Quarantinetime++;
                if (person[i].Quarantinetime > 13) {
                    person[i].status = person[i].status ^ 0x00000002;
                }
            }
            if (person[i].Infectiontime > 13) {
                DeathChance = rand() % 10001;
                if (((person[i].status & 0x3F800000) >> 23) < 13) {
                    if (DeathChance < 6) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                else if (((person[i].status & 0x3F800000) >> 23) < 18) {
                    if (DeathChance < 10) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                else if (((person[i].status & 0x3F800000) >> 23) < 30) {
                    if (DeathChance < 10) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                else if (((person[i].status & 0x3F800000) >> 23) < 50) {
                    if (DeathChance < 61) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                else if (((person[i].status & 0x3F800000) >> 23) < 70) {
                    if (DeathChance < 413) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                else {
                    if (DeathChance < 2491) {
                        person[i].status = person[i].status | 0x40000000;
                        //printf("Person %d died\n", i);
                    }
                }
                person[i].status = person[i].status ^ 0x80000000;
                person[i].status = person[i].status | 0x00000001;
            }
        }
    }
    return;
}


long int Count_Infected(long int population_size) {
    int Infected_Count = 0;
    for (int i = 0; i < population_size; i++) {
        if ((person[i].status & 0x80000000) == 0x80000000) {
            Infected_Count++;
        }
        else if ((person[i].status & 0x00000001) == 0x00000001) {
            Infected_Count++;
        }
    }
    return Infected_Count;

}
long int Count_Dead(long int population_size) {
        int Dead_Count = 0;
        for (int i = 0; i < population_size; i++) {
            if ((person[i].status & 0x40000000) == 0x40000000) {
                Dead_Count++;
            }
        }
        return Dead_Count;
}
void Event_Party(long int Under_18_age_count, long int Under_Adult_age_count) {
    People *attendee[200];
    for (int cl = 0; cl < 200; cl++) {
        attendee[cl] = 0;
    }
    People* random_Person;
    int infected_count = 0;
    for (int i = 0; i < 200; i++) {
        do {
            int check_count=0;
            for (long int ch = Under_18_age_count; ch < (Under_Adult_age_count+ Under_18_age_count); ch++) {
                if ((person[ch].status & 0x00000004) == 0x00000004) {
                    check_count++;
                }
            }
            if (check_count > ( Under_18_age_count-1)) {
                goto finish;
            }
            random_Person = person + ((rand() * rand()) % (Under_Adult_age_count) + Under_18_age_count);
            if (random_Person == 0)continue;
            if ((random_Person->status & 0x00000004) != 0x00000004) {
                random_Person->status = random_Person->status | 0x00000004;
                attendee[i] = random_Person;
                if ((random_Person->status & 0x80000000) == 0x80000000) {
                    infected_count++;
                    
                }
                goto end;
            }
            else continue;
            
        } while ((random_Person->status & 0x00000004) != 0x00000004);
        end:;
    }
finish:;
    for (int i = 0; i < 200; i++) {
        int Chance = rand() % 10000;
        float INF_Chance = (infected_count / (float)10) * Rval;
        if (Chance < INF_Chance) {
            if (attendee[i] != 0) {
                attendee[i]->status = attendee[i]->status | 0x80000000;
                printf("Person %ld got infected at the party\n", (attendee[i] - person));
            }
        }
    }

    return;
}

int Prompt(long int * days, long int population_size, int current_day) {
    char input;
    long int count;
    do {
        printf("\n\n\nPress 'a' to see all the people in the array.\n");
        printf("Press 's' to see some the people in the array.\n");
        printf("Press 'n' to move on to the next day.\n");
        printf("Press 'c' to see the number of new infections and new deaths.\n");
        printf("Press 't' to see the total number of infections and deaths.\n");
        printf("Press 'q' to end the program.\n");
        printf("Press 'w' to go through a week\n");
        printf("Press 'p' to set parties for the day\n");
        printf("Press 'b' to change the testing chance and quarantine rules\n");
        printf("Press 'd' to see the number of dead people\n");
        scanf(" %c", &input);
        switch (input) {
        case 'a':
            Print_Everyone(population_size);
            system("Pause");
            break;
        case 's':
            Print_People_Div(population_size);
            system("Pause");
            break;

        case 'b':
            return 3;
        case 'w':
            return 2;
        case 'n':
            return 1;
        case 'q':
            return 0;
        case 'c':
            printf("The number of new infections today is %ld\n", days[current_day]);//Finding the difference between this day and the day before to get the new infections.
            system("Pause");
            break;
        case 't':
            count = Count_Infected(population_size);
            printf("The total number of infected people is: %ld\n", count);
            system("Pause");
            break;
        case 'p':
            return 4;
        case 'd':
            count = Count_Infected(population_size);
            printf("The total number of dead people is: %ld\n", Count_Dead(population_size));
            system("Pause");
            break;
        default:
            printf("Invalid Input\n");
            break;
        }
       
    } while (input != 'q');
        return 0;
}


  /*
    Values used:
    Looking at Data from May 2020 to Aug 2020.
    I can find a form of mortality rate based on cases of COVID-19 based on age and the deaths of COVID-19 based on age.
    
    Source where I found the age disparity of cases within the United States (From May 2020 -> Aug 2020):
    https://www.cdc.gov/mmwr/volumes/69/wr/mm6939e1.htm
    Source where I found the age disparity of deaths within the United States (I added the numbers form May 2nd -> Aug 29th)
    https://www.cdc.gov/nchs/nvss/vsrr/covid_weekly/index.htm

    Data I found:
                    Chance of Death given infected with COVID-19.
    Ages 0->12:	                0.06441% 
    Ages 13->17:	            0.09581%   
    Ages 18->29:	            0.09557%
    Ages 30->49:	            0.60765%
    Ages 50->69:	            4.13185%
    Ages 70->100:	            24.91779%

    Scrapping the health idea and just using this as a standard for deaths
    In terms of severity of desease we can calculate that based on age. (Used only for contagousness)*/















    /*char debug = getchar();
    if (debug == 's') {
        for (int i = 0; i < 200; i++) {
            printf("********************************\n");
            printf("Person %d   Status:  0x%X   :::::: Age: %d \n", (i), person[i].status, (person[i].status >> 23));
            printf("Person %d   House:  0x%lX \n", (i), person[i].house);
            printf("Person %d   Work:  0x%lX \n", (i), person[i].work);
            printf("Person %d   Social:  0x%lX \n", (i), person[i].social);
            printf("********************************\n\n");
        }
    }*/






