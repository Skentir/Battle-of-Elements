/*
    Name: Sison, Danielle Kirsten T.
    Section: S12B
    This machine project is a requirement for CCPROG2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

typedef char String[100];
typedef char LongString[250];

void main_menu(), character_menu(),  moves_menu(), print_hero(), print_move(), element_converter(), generate_move();
void move_choice_list(), begin_battle(), print_contenders(),  print_header(), freeplay_menu(), print_storyline();
void add_moves(), print_gameset(), delete_gameset(), view_storybyHero(), hero_choice_list(), set_color(), esc_color();
int get_heroindex(), get_moveIndex(), get_hsize(), get_msizeb(), proceed_round();
void set_hero_choice_list();

typedef struct Characters {
    String hero_name;
    String stage_name;
    int id;
    int element;
    int health;
    int power;
    int currHP, curPP;
} Hero;
typedef Hero Heroes[300];

typedef struct MoveTag {
    int move_elem;
    String move_name;
    String description;
    int move_id;
    int move_pp;
    int move_type;
    int move_min, move_max;
} Move;
typedef Move Moves[300];

typedef struct moveSetTag {
    int heroID;
    int moveID[3];
} Set;
typedef Set MoveSets[900];

typedef struct StoryTag {
    int story_ID;
    String story_name;
    String description;
    int main_hero;
    int size;
    int enemy_list[300];
} Story;
typedef Story Stories[300];

typedef struct EnvironmentTag {
    int elem_background;
    LongString design[12]; //String[12][200]
} Environment;
typedef Environment Backgrounds[10];

//Counter functions
int element_counter(Moves move_list, int m_size, int elem)
{
/* This functions returns the number of times, a
 * certain element occured */
    int i, count = 0;
    for(i = 0; i < m_size; i++) {
        if(move_list[i].move_elem == elem) {
            count++;
        }
    } return count;
}
int enemy_repeat_checker(Story story, int enemy_id, int size)
{
    /* This functions returns the number of times, a
     * certain enemy appeared */
    int i, counter = 0;
    for(i=0; i < size; i++)
        if (story.enemy_list[i] == enemy_id)
            counter++;
    return counter;
}
int moveset_checker(Set move_set, int move_id)
{
    /* This functions returns -1 if a move is assigned to a
     * character and 0 if not */
    int i;
    for (i=0; i<3; i++)
        if(move_set.moveID[i] == move_id)
            return -1;
    return 0;
}
//Game Set functions
void set_moves(MoveSets set_list, Heroes hero_list, Moves move_list, int heroID, int m_size, int *setsize, int defsize)
{
    /* This functions allows the user to assign moves to a character.
     * If a character has no assigned moves, it will assign and append to txt file.
     * If a character has assigned moves, it will delete and prompt user, then append to txt file.
     * Called in add_hero() and edit_hero(); */
    
    int i, j, s_index = -1, count, hero_elem, h_index, move_id, move_index, move_elem, valid;

    // Check if heroID has existing assigned moves
    for (i = 0; i < *setsize; i++)
        if (set_list[i].heroID == heroID)
            s_index = i;

    if (s_index == -1) {
        printf("Oh no, this hero doesn't have an assigned move set!\nDon't worry...\n");
        set_list[*setsize].heroID = heroID;
        (*setsize)++;
        s_index = *setsize - 1;
    } else {
        printf("Removing moves assigned to this hero...\n");
        delete_gameset(set_list, setsize, heroID); // Delete the hero in moveset, append new ver. of hero at the end
        set_list[*setsize].heroID = heroID;
        s_index = *setsize;
    }

    FILE *gameset_add;
    gameset_add = fopen("gameset.txt", "a");

    if(gameset_add != NULL) {

        h_index = get_heroindex(hero_list, heroID, defsize);
        printf("Hero %d at index %d,", heroID, h_index);
        hero_elem = hero_list[h_index].element;
        printf("has element: [%d] ", hero_elem); element_converter(hero_elem); printf("\n");
        
        // Check if there exists at least 3 moves of a particular element
        count = element_counter(move_list, m_size,hero_elem);
        printf("\nThere are %d moves of element: ", count); element_converter(hero_elem); printf("\n");
        int move_case = 2;
        move_choice_list(move_list, m_size, move_case, hero_elem);
       // If there are at least 3, user can assign directly
        if (count >= 3) {
            for (j = 0; j < 3; j++) {
                do {
                    printf("\nEnter a Move ID of element: ");
                    element_converter(hero_elem); printf("\n");
                    printf("Choice: "); scanf("%d", &move_id);
                    move_index = get_moveIndex(move_list, move_id, m_size);
                    move_elem = move_list[move_index].move_elem;
                    printf("Move %d at index %d has element [%d] ", move_id, move_index, move_elem); element_converter(move_elem);
                    printf("\n");

                    valid = moveset_checker(set_list[s_index], move_id); // returns -1 if move is already in the list
                    if (valid == -1)
                        printf("That move is already assigned. Pick again.\n");
                } while (move_index == -1|| move_elem != hero_elem || valid == -1);

                set_list[s_index].moveID[j] = move_id;
            }
            print_gameset(set_list[s_index], gameset_add);
        } else {
            // If less than 3, user should add more moves of that element before assigning
            do {
                printf("You should add a move of element: ");
                element_converter(hero_elem); printf("\n");
                add_moves(move_list, &m_size, hero_list, defsize,set_list, setsize); // Add move 3 (or less) times before prompting user to assign moves to selected hero
                count = element_counter(move_list, m_size, hero_elem);
            } while (count < 3);

            for (j = 0; j < 3; j++) {
                do {
                    printf("\nEnter a Move ID of element: ");
                    element_converter(hero_elem); printf("\n");
                    printf("Choice: "); scanf("%d", &move_id);
                    move_index = get_moveIndex(move_list, move_id, m_size);
                    move_elem = move_list[move_index].move_elem;
                    printf("Move %d at index %d has element [%d] ", move_id, move_index, move_elem);
                    element_converter(move_elem); printf("\n");
                    valid = moveset_checker(set_list[s_index], move_id);
                    if (valid == -1)
                        printf("That move is already assigned. Pick again.\n");
                } while (move_index == -1 || move_elem != hero_elem || valid == -1);

                set_list[s_index].moveID[j] = move_id;
            }
            print_gameset(set_list[s_index], gameset_add);
        }
        fclose(gameset_add);
    } else
        printf("ERROR accessing file.\n");
}
void check_move(Heroes hero_list, int defsize, Moves move_list, int m_size, int deleted_id, MoveSets set_list, int setsize)
{
    /* This function checks if move is assigned to a character.
     * If so, it will generate a random move.
     * Called in delete_hero() and edit_move(); */
    
    FILE *gameset_edit;
    gameset_edit = fopen("gameset.txt", "wt");
    int i,j; // Call in delete_move() and edit_move();
    for(i=0; i< setsize; i++) {
        for(j=0; j<3; j++) {
            if(set_list[i].moveID[j] == deleted_id) {
                set_color(91); printf("System found a hero of ID %d with the deleted move ID.\n", set_list[i].heroID); esc_color();
                // System generate a move via generate_move
				int h_index; 
				h_index = get_heroindex(hero_list, set_list[i].heroID, defsize);
				int elem = hero_list[h_index].element;
				int counter = element_counter(move_list, m_size, elem);
                // Make sure system has enough number of moves to choose
				while (counter < 3) {
					add_moves(move_list, &m_size, hero_list, defsize, set_list, setsize);
					counter = element_counter(move_list, m_size, elem);
				}
                 generate_move(hero_list, defsize, &set_list[i], move_list, m_size, deleted_id, j);
            }
        }
        print_gameset(set_list[i], gameset_edit); // Print per set
    }
    fclose(gameset_edit);
}
//Generator functions
int sys_rand(int size)
{
    //Generates Random Number with 'size' as limit
    srand(time(NULL));
    return (1+(rand()%size));
}
void generate_move(Heroes hero_list, int defsize, Set *gameset, Moves move_list, int m_size, int deleted_id, int deletedID_index)
{
    /* This functions finds a move of similar element to the hero
     * Makes sure move is not the same as other moves assigned to the hero
     * Called in check_move(); */
    int occurance, i, j, element = hero_list[get_heroindex(hero_list, gameset->heroID, defsize)].element;

    do {
        occurance = 0;
        i = sys_rand(m_size)-1;

        if(move_list[i].move_elem == element) {
            for (j = 0; j < 3; j++) {
                if (move_list[i].move_id == gameset->moveID[j])
                    occurance++; // How many times generated num occurred in the array
            }
            if (occurance == 0) {
                gameset->moveID[deletedID_index] = move_list[i].move_id;
                set_color(91);printf("System has succesfully generated a move of ID %d\n", move_list[i].move_id); esc_color();
            }
        } else
            occurance = 4; // means diff element
    } while (occurance > 0);
}
void generate_enemy(Story *story, int enemy_index, int deletedID, MoveSets set_list, int setsize)
{
    /* This functions replaces the deleted enemy in a storyline
     * Called in delete_story_enemy(); */
    int i, j, found = 0;
    //Find a character not appearing in the same array in enemy_list
    while(found != 1) {
        i = sys_rand(setsize)-2;  // Generate an random index
        if (set_list[i].heroID != deletedID) { // Check if same to deleted ID
            for (j = 0; j < story->size; j++) {
                if (set_list[i].heroID != story->enemy_list[j] && set_list[i].heroID != story->main_hero) // Check if it has a duplicate in storyline's enemy list
                    found = 1;
                else
                    found = 0;
            }

            if (found == 1) {
                story->enemy_list[enemy_index] = set_list[i].heroID;
                set_color(91);
                printf("System has succesfully generated ID %d an enemy for Story ID %d.\n", set_list[i].heroID, story->story_ID);
                esc_color();
            }
        }
    }
}
//Error Checker functions
int get_heroindex(Heroes hero_list, int h_id, int defsize)
{
    /* Returns index of Hero given an ID*/
    int i;
    for(i=0; i< defsize;i++)
        if (hero_list[i].id == h_id)
            return i;
    return -1;
}
int get_moveIndex(Moves move_list, int m_id, int m_size)
{
    /* Returns index of Move given an ID*/
    int i;
    for(i=0; i< m_size;i++)
        if(move_list[i].move_id == m_id)
            return i;
    return -1;
}
int get_gamesetIndex(MoveSets set_list, int g_id, int setsize)
{
    /* Returns index of Move Set given an ID*/
    int i;
    for(i=0; i < setsize;i++)
        if(set_list[i].heroID == g_id)
            return i;
    return -1;
}
int get_storyIndex(Stories story_list, int s_id, int strsize)
{
    /* Returns index of Story given an ID*/
    int i;
    for (i=0; i < strsize; i++)
        if(story_list[i].story_ID == s_id)
            return i;
    return -1;
}
int story_hero_exists(Stories story_list, int h_id, int strsize)
{
    /* Returns -1 if heroID is not a hero in any storyline */
    int i;
    for (i=0; i < strsize; i++)
        if(story_list[i].main_hero == h_id)
            return i;
    return -1;
}
int story_enemy_exists(Stories story_list, int e_id, int strsize)
{
    /* Returns -1 if heroID is not an enemy in any storyline */
    int i,j;
    for (i=0; i < strsize; i++) {
        for (j=0; j < story_list[i].size; j++) {
            if (story_list[i].enemy_list[j] == e_id) {
                return i;
            }
        }
    }
    return -1;
}
int enemy_repeat(Story storyline, int enemy_id)
{
    /* Returns -1 if enemy is already an enemy in a storyline */
    int i = 0;
    for(i=0; i < storyline.size; i++) {
        if(enemy_id == storyline.enemy_list[i])
            return -1;
    }
    return 0;
}
//Add functions
void add_moves(Moves move_list, int *m_size, Heroes hero_list, int defsize, MoveSets set_list, int setsize)
{
    /* This function adds a move and appends it in moves.txt */
    system("cls");
    FILE *moves_add;
    moves_add = fopen("moves.txt", "a");
    String m_name, m_description; int m_elem, m_type, m_pp, m_min, m_max;

    if (moves_add != NULL) {
        int m;
        printf("What will you call your move?\n"); scanf(" ");

        do {
            fgets(m_name, sizeof(m_name), stdin);
        } while(strlen(m_name)>100);
        m_name[strlen(m_name)-1] = '\0';

        printf("What would the move description be?\n");
        do{
            fgets(m_description, sizeof(m_description), stdin);
        } while(strlen(m_description)>100);
        m_description[strlen(m_description)-1] = '\0';

        do {
            printf("Will this be a ...\n[1] Light Attack\n[2] Medium Attack\n[3] Heavy Attack\n");
            printf("Choice :"); scanf("%d",&m_type);
        } while (m_type < 1 || m_type > 3);

        do {
            printf("Maximum is 180. Minimum is 20. Difference of Min. and Max. should not exceed 60.\n");
            printf("Min. Damage:"); scanf("%d",&m_min);
            printf("Max Damage:");scanf("%d", &m_max);
        } while (m_max-m_min>60 || m_max > 180 || m_min < 20 || m_max == m_min || m_min > m_max);

        do {
            printf("Minimum 10. Maximum 40\n");
            printf("How much PP will this move cost?\n");
            printf("Choice: "); scanf("%d", &m_pp);
        } while (m_pp < 10 || m_pp > 40);

        do {
            printf("Now...choose the element of your move!\n");
            for (m=1; m<=4;m++) {
                printf("[%c[%dm%d", 0x1B, 96, m);
                printf("] "); element_converter(m); printf("\n");
            }
            printf("Choice: "); scanf("%d", &m_elem);
        } while(m_elem < 1 || m_elem > 4);

        move_list[*m_size].move_elem = m_elem;
        strcpy(move_list[*m_size].move_name, m_name);
        strcpy(move_list[*m_size].description, m_description);
        if (*m_size == 0)
        	move_list[*m_size].move_id = 1;
        else
        	move_list[*m_size].move_id = move_list[*m_size-1].move_id + 1;
        move_list[*m_size].move_pp = m_pp;
        move_list[*m_size].move_type = m_type;
        move_list[*m_size].move_max = m_max;
        move_list[*m_size].move_min = m_min;

        printf("Move new id is : %d\n",move_list[*m_size].move_id);
        print_move(move_list[*m_size],moves_add);

        (*m_size)++;
        fclose(moves_add);
    } else printf("ERROR opening file.\n");
}
void add_hero(Heroes hero_list, int *defsize)
{
    /* This function adds a hero and appends it in character.txt */
    system("cls");
    FILE *character_add;
    character_add = fopen("character.txt", "a");
    String c_name, c_stage; int c_hp, c_pp, c_element, i;

    if (character_add != NULL) {
        printf("What will you call your hero?\n"); scanf(" ");

        do {
            fgets(c_name, sizeof(c_name), stdin);
        } while(strlen(c_name)>100);
        c_name[strlen(c_name)-1] = '\0';

        printf("What would the stage name be?\n");
        do{
            fgets(c_stage, sizeof(c_stage), stdin);
        } while(strlen(c_stage)>100);
        c_stage[strlen(c_stage)-1] = '\0';

        do {
            printf("Maximum is 500. Minimum is 50.\n");
            printf("HP:"); scanf("%d",&c_hp);
        } while (c_hp < 50 || c_hp > 500);

        do {
            printf("Maximum is 40. Minimum is 10.\n");
            printf("PP:");
            scanf("%d", &c_pp);
        } while (c_pp < 10 || c_pp > 40);


        do {
            printf("Now...choose what element your character will possess!\n");
            for (i=1; i<=4;i++) {
                printf("[%c[%dm%d", 0x1B, 96, i);
                printf("] "); element_converter(i); printf("\n");
            }

            printf("Choice: "); scanf("%d", &c_element);
        } while(c_element < 1 || c_element > 4);

        hero_list[*defsize].element = c_element;
        if (*defsize == 0)
        	hero_list[*defsize].id = 1;
        else
        	hero_list[*defsize].id = hero_list[*defsize-1].id + 1;
        strcpy(hero_list[*defsize].hero_name, c_name);
        strcpy(hero_list[*defsize].stage_name, c_stage);
        hero_list[*defsize].health = c_hp;
        hero_list[*defsize].currHP = c_hp;
        hero_list[*defsize].power = c_pp;
        hero_list[*defsize].curPP = c_pp;
        printf("Character new id is : %d\n",hero_list[*defsize].id);


        print_hero(hero_list[*defsize],character_add);
        (*defsize)++;
        fclose(character_add);
    } else printf("ERROR opening file.\n");
}
void add_storyline(Stories story_list, int *strsize, Heroes hero_list, int defsize, MoveSets set_list, int setsize, Moves move_list, int m_size)
{
        /* This function adds a storyline and appends it in story.txt */
        String name, description; int num_battles = 0, i,j, max, hero_id, enemy_id, occurance, gs_index, hero_set_index;
        FILE *story_add;
        story_add = fopen("storyline.txt","a");

        if(story_add != NULL) {
            do {
                printf("\nEnter a story name: "); scanf(" ");
                fgets(name, sizeof(name), stdin);
                if(strlen(name) > 100)
                    printf("Too long!\n");
            } while(strlen(name) > 100);
            name[strlen(name) - 1] = '\0';
            strcpy(story_list[*strsize].story_name, name);

            do {
                printf("Enter a description: ");
                fgets(description, sizeof(description), stdin);
                if(strlen(description)>100)
                    printf("Too long!\n");
            } while(strlen(description)>100);
            description[strlen(description) - 1] = '\0';
            strcpy(story_list[*strsize].description, description);

            /* Make sure to have enought heroes with assigned moves.
             * 4 is the minimum assuming 1 will be the main hero, and the other 3 are villains.
             * If less than 3, assign moves to a hero. */
            if (setsize >= 4) {
                do {
                    printf("\nEnter number of battles to take place: ");
                    scanf("%d", &num_battles);
                    max = setsize - 1;
                    // # of Heroes with Assigned Moves is the Maximum # of Battles. Exclude the protagonist, hence the -1.
                    if(num_battles < 3 || num_battles > max)
                        printf("Note: Minimum is 3 and Maximum is %d.\n", max);
                } while (num_battles < 3 || num_battles > max);
                story_list[*strsize].size = num_battles;
            } else {
                // When less than 4 heroes have assigned moves
                printf("\nYou have to assign moves to %d characters.\n", 4-setsize);
               hero_in_set_choice_list(hero_list, defsize, set_list, setsize); //Heroes with no assigned moves
                do {
                    printf("Choice: "); scanf("%d", &hero_id);
                    hero_set_index = get_gamesetIndex(set_list, hero_id, setsize);
                } while(hero_set_index != -1);
                for (j=0; j< 3-setsize; j++) {
                    set_moves(set_list, hero_list, move_list, hero_id, m_size, &setsize, defsize);
                }

                do {
                    printf("\nEnter number of battles to take place: ");
                    scanf("%d", &num_battles);
                    max = setsize - 1;
                    // # of Heroes with Assigned Moves is the Maximum # of Battles. Exclude the protagonist, hence the -1.
                    printf("Note: Minimum is 3 and Maximum is %d.\n", max);
                } while (num_battles < 3 || num_battles > max);
                story_list[*strsize].size = num_battles;
            }

            printf("\nWho will be the main superhero?\n");
            set_hero_choice_list(set_list, setsize, hero_list, defsize);

            do {
                printf("Enter Hero ID: "); scanf("%d", &hero_id);
                if (get_gamesetIndex(set_list, hero_id, setsize) == -1)
                    printf("This character has no assigned moves. Pick again.\n");
            } while(get_gamesetIndex(set_list, hero_id, setsize) == -1);
            story_list[*strsize].main_hero = hero_id;

            printf("\nEnter %d Hero IDS to be your villain\n", num_battles);
            hero_choice_list(hero_list, defsize);
            for(i=0; i < num_battles; i++) {
                do {
                    printf("Villain ID #%d: ", i+1); scanf("%d", &enemy_id);
                    //Check if enemy ID repeats in List of Villains to be fought. Return value of 0 if valid.
                    occurance = enemy_repeat_checker(story_list[*strsize], enemy_id, i);
                    //Check if enemy ID has assigned moves
                    gs_index = get_gamesetIndex(set_list,enemy_id,setsize);
                    printf("Game Set index: %d\n", gs_index);

                    if(enemy_id == hero_id)
                        printf("That's your protagonist already! Pick again.\n");
                    else if (occurance > 0)
                        printf("You selected that villain already! Pick again.\n");
                    else if (gs_index == -1)
                        printf("Selected ID has no assigned moves! Pick again.\n");

                } while(gs_index == -1 || enemy_id == hero_id|| occurance > 0);
                story_list[*strsize].enemy_list[i] = enemy_id;
            }
			
			if (*strsize == 0)
				story_list[*strsize].story_ID = 1;
			else
            	story_list[*strsize].story_ID = story_list[*strsize-1].story_ID + 1;
            print_storyline(story_list[*strsize], story_add);
            (*strsize)++;
            printf("New Storyline ID: %d\n", story_list[*strsize].story_ID);
            fclose(story_add);
        } else
            printf("Error accessing file.\n");
}
//Converter functions
void movetype_converter(int n)
{
    /* This function accepts an integer and converts it to corresponding move type */
    switch (n) {
        case 1: printf("Light Attack"); break;
        case 2: printf("Medium Attack"); break;
        case 3: printf("Heavy Attack"); break;
        default: printf("Unknown type attack."); break;
    }
}
void element_converter(int n)
{
    /* This function accepts an integer and converts it to corresponding element */
    switch(n)
    {
        case 1:
            set_color(34); printf("Water"); break;
        case 2:
            set_color(91); printf("Fire"); break;
        case 3:
            set_color(37); printf("Air"); break;
        case 4:
            set_color(92); printf("Earth"); break;
        default: printf("Unknown Element");
    }
    esc_color();
}
void heroID_to_name_converter(Heroes hero_list, int h_id, int defsize)
{
    /* This function accepts a hero ID and prints its corresponding name */
    int h_index;
    h_index = get_heroindex(hero_list, h_id, defsize);
    puts(hero_list[h_index].hero_name);
}
//View By ID functions
void view_herobyID(Heroes hero_list, int hero_index)
{
    /* This function accepts an index of an ID and prints hero details */
    printf("\nElement: ");
    element_converter(hero_list[hero_index].element); printf("\n");
    printf("\n");
    puts(hero_list[hero_index].hero_name);
    puts(hero_list[hero_index].stage_name);
    printf("Max Health: %d\n", hero_list[hero_index].health);
    printf("Max Power: %d\n", hero_list[hero_index].power);
}
void view_movebyID(Moves move_list, int move_index)
{
    /* This function accepts an index of an ID and prints move details */
    printf("\nElement: ");element_converter(move_list[move_index].move_elem);
    printf("\n");
    puts(move_list[move_index].move_name);
    puts(move_list[move_index].description);
    printf("PP requirement: %d\n", move_list[move_index].move_pp);
    movetype_converter(move_list[move_index].move_type);
    printf("\nMin. Damage: %d\n", move_list[move_index].move_min);
    printf("Max. Damage: %d\n\n", move_list[move_index].move_max);
}
void view_storybyID(Stories story_list, int str_index, Heroes hero_list, int defsize)
{
    /* This function accepts an index of an ID and prints story details */
    int j;
    printf("\n");
    puts(story_list[str_index].story_name);
    puts(story_list[str_index].description);
    printf("Main Hero: "); // Print hero name
    heroID_to_name_converter(hero_list, story_list[str_index].main_hero, defsize);
    printf("\n");
    printf("Number of Stages: %d\n", story_list[str_index].size); printf("\n");
    printf("Enemies: \n");
    for(j=0; j < story_list[str_index].size; j++)
        heroID_to_name_converter(hero_list, story_list[str_index].enemy_list[j], defsize); // Print enemy names
    printf("\n");
}
//View By Element functions
void view_herobyElement(Heroes hero_list, int defsize, int h_elem)
{
    /* This function accepts an element (represent by an int) and prints hero details */
    int i;
    for(i=0; i < defsize; i++)
    {
        if(hero_list[i].element == h_elem)
        {
            element_converter(hero_list[i].element);
            printf("\n");
            puts(hero_list[i].hero_name);
            puts(hero_list[i].stage_name);
            printf("Max Health: %d\n", hero_list[i].health);
            printf("Max Power: %d\n", hero_list[i].power);
        }
    }
}
void view_movebyElement(Moves move_list, int m_size)
{
    /* This function accepts an element (represent by an int) and prints move details */
    int m_elem, i;

    for (i=1; i<=4;i++) {
        printf("[%c[%dm%d", 0x1B, 96, i);
        printf("] "); element_converter(i); printf("\n");
    }

    do {
        printf("Enter Move Element: ");scanf("%d", &m_elem);
    } while(m_elem < 1 || m_elem > 4);

    for(i=0; i< m_size; i++)
    {
        if(move_list[i].move_elem == m_elem) {
            printf("Element: ");
            element_converter(move_list[i].move_elem);
            printf("\n");
            puts(move_list[i].move_name);
            puts(move_list[i].description);
            printf("PP requirement: %d\n", move_list[i].move_pp);
            movetype_converter(move_list[i].move_type);
            printf("\nMin. Damage: %d\n", move_list[i].move_min);
            printf("Max. Damage: %d\n\n", move_list[i].move_max);
        }
    }
}
void view_backgroundbyElement(Backgrounds background_list, int bg_size, int h_elem)
{
   /* This function accepts an element (represent by an int) and prints background */
    int i,j;
    for(i=0; i < bg_size; i++) {
        if(background_list[i].elem_background == h_elem) {
            for(j=0; j < 12; j++)
                puts(background_list[i].design[j]);
        }
    }
}
//View By Enemy/Protagonist function
void view_storybyEnemy(Stories story_list, int strsize, Heroes hero_list, int defsize)
{
    /* This function prints storylines depending on the enemy */
    int v_id, i, j, found = 0;
    do {
        printf("Enter ID of villain: "); scanf("%d", &v_id);
        i = story_enemy_exists(story_list, v_id,strsize);
    } while (i == -1);

    for (i = 0; i < strsize; i++) {
        found = 0;
        for (j = 0; j < story_list[i].size; j++) {
            if (story_list[i].enemy_list[j] == v_id)// Print enemy names
                    found = 1;
        }
        if(found == 1) {
            printf("\nStory ID: %d\n", story_list[i].story_ID);
            puts(story_list[i].story_name);
            puts(story_list[i].description);
            printf("Main Hero:"); // Print hero name
            heroID_to_name_converter(hero_list, story_list[i].main_hero, defsize);
            printf("\n");
            printf("Number of Stages: %d\n", story_list[i].size); printf("\n");
            printf("Enemies: \n");
            for(j=0; j <story_list[i].size; j++)
                heroID_to_name_converter(hero_list, story_list[i].enemy_list[j], defsize); //Print enemy names
            printf("\n");
        }
    } printf("\n");
}
void view_storybyHero(Stories story_list, int strsize, Heroes hero_list, int defsize)
{
     /* This function prints storylines depending on the main hero */
    int h_id, i, j;
    do {
        printf("Enter ID of Main Hero: "); scanf("%d", &h_id);
        j = story_hero_exists(story_list, h_id, strsize);
    } while (j == -1);

     printf("\nStory ID: %d\n", story_list[j].story_ID);
     puts(story_list[j].story_name);
     puts(story_list[j].description);
     printf("Main Hero: "); // Print hero name
     heroID_to_name_converter(hero_list, story_list[j].main_hero, defsize);
     printf("\n");
     printf("Number of Stages: %d\n", story_list[j].size); printf("\n");
     printf("Enemies: \n");
     for(i=0; i <story_list[j].size; i++)
         heroID_to_name_converter(hero_list, story_list[j].enemy_list[i], defsize); // Print enemy names
     printf("\n");
}
//View Menu functions
void view_hero(Heroes hero_list, int defsize)
{
     /* This function is a menu function for view hero */
    system("cls");
    print_header("VIEW HERO");
    FILE *character_play;
    character_play = fopen("character.txt", "rt");
    int v_choice, i;
    if(character_play != NULL) {
        do {
            printf("View Hero by...\n");
            set_color(96); printf("[1] "); esc_color(); printf("Hero ID\n");
			set_color(96); printf("[2] "); esc_color(); printf("Elemental Affinity\n");
            printf("Choice: "); scanf("%d", &v_choice);
        } while (v_choice < 1 || v_choice > 2);

        if (v_choice == 1) {
            int h_id;
            hero_choice_list(hero_list, defsize);
            do {
                printf("Enter Hero ID: "); scanf("%d", &h_id);
                i = get_heroindex(hero_list,h_id, defsize);
            } while(i == -1);

            view_herobyID(hero_list, i);
        }
        else {
            int h_elem;
            printf("\nThe elements are...\n");
            for (i=1; i<=4;i++) {
                set_color(96); printf("[%d] ", i); esc_color();
                element_converter(i); printf("\n");
            }
            do { printf("Enter Element: "); scanf("%d", &h_elem); } while(h_elem < 1 || h_elem > 4);
            printf("\n");
            view_herobyElement(hero_list, defsize, h_elem);
        }
        fclose(character_play);
    } else printf("ERROR accessing file");
}
void view_moves(Moves move_list, int m_size)
{
    /* This function is a menu function for view moves */
    system("cls");
    print_header("VIEW POWERS");
    FILE *move_play;
    move_play = fopen("moves.txt", "rt");
    if(move_play != NULL) {
        int m_choice;

        do {
            printf("View Superpower by...\n");
            set_color(96); printf("[1]"); esc_color(); printf("Superpower ID\n");
            set_color(96); printf("[2]"); esc_color(); printf("Elemental Affinity\n");
            printf("Choice: "); scanf("%d", &m_choice);
        } while (m_choice < 1 || m_choice > 2);

        if (m_choice == 1) {
            int m_id, i, m_case = 3;
            move_choice_list(move_list, m_size, m_case, 0);
            do {
                printf("Enter Move ID: ");
                scanf("%d", &m_id);
                i = get_moveIndex(move_list, m_id, m_size);
            } while(i == -1);
            printf("\n");
            view_movebyID(move_list, i);
        }
        else
            view_movebyElement(move_list, m_size);
        fclose(move_play);
    } else printf("ERROR accessing file.\n");
}
void view_storyline(Stories story_list, int strsize, Heroes hero_list, int defsize)
{
    /* This function is a menu function for view storylines */
    system("cls");
    print_header("VIEW A STORY");
    FILE *story_play;
    story_play = fopen("storyline.txt","rt");
    if(story_play != NULL) {
        int s_choice;
        do {
            printf("View Storylines by...\n");
            set_color(96); printf("[1]"); esc_color(); printf(" Main Hero\n");
            set_color(96); printf("[2]"); esc_color(); printf(" Enemy\n");
            set_color(96); printf("[3]"); esc_color(); printf(" Story ID\n");

            printf("Choice: "); scanf("%d", &s_choice);
        } while (s_choice<1 || s_choice>3);

        if (s_choice == 1)
            view_storybyHero(story_list, strsize, hero_list, defsize);
        else if(s_choice == 3) {
            int s_id, i;
            do {
                printf("Enter Story ID: "); scanf("%d", &s_id);
                i = get_storyIndex(story_list, s_id, strsize);
            } while(i == -1);
            view_storybyID(story_list, i, hero_list, defsize);
        } else
            view_storybyEnemy(story_list,strsize, hero_list, defsize);

        fclose(story_play);
    } else
        printf("ERROR accessing file.\n");
}
void hero_choice_list(Heroes hero_list, int defsize)
{
    /* This function is a menu function for viewing heroes */
	int h;
		printf(" ID          Name\n");
	for(h=0; h < defsize; h++) {
    	set_color(96); printf("[%d]", hero_list[h].id);
    	esc_color(); printf(" %s\n", hero_list[h].hero_name);
	}
}
void hero_in_set_choice_list(Heroes hero_list, int defsize, MoveSets set_list, int setsize)
{
     /* This function is a menu function for viewing heroes with no assigned moves */
int h, s, found;
printf(" ID          Name\n");
for(h=0; h < defsize; h++) {
	found = 0;
	for(s=0; s < setsize; s++) {
    	if(hero_list[h].id == set_list[s].heroID)
    		found = 1;
	}
	if (found == 0) {
		set_color(96); printf("[%d]", hero_list[h].id);
    	esc_color(); printf(" %s\n", hero_list[h].hero_name);
	}
}
printf("\n");
}
void move_choice_list(Moves move_list, int m_size, int move_case, int elem)
{
     /* This function is a menu function for viewing names of moves depending on the case
      * Case 1: Print all types of moves
      * Case 2: Print moves depending on element
      * Case 3: Print all types of moves with element */
    int m;
    printf(" ID          Name\n");
    if (move_case == 3) {
        for (m = 0; m < m_size; m++) {
            set_color(96); printf("[%d]", move_list[m].move_id); esc_color();
            printf(" %s - ", move_list[m].move_name);
            element_converter(move_list[m].move_elem); printf("\n");
        }
    } else if (move_case == 1) {
        for (m = 0; m < m_size; m++){
            set_color(96); printf("[%d]", move_list[m].move_id);
            esc_color(); printf(" %s\n", move_list[m].move_name);
        }
        printf("\n");
    } else {
        for (m = 0; m < m_size; m++) {
            if(move_list[m].move_elem == elem) {
                set_color(96); printf("[%d]", move_list[m].move_id);
                esc_color(); printf(" %s\n", move_list[m].move_name);
            }
        }
        printf("\n");
    }
}
void storyline_choice_list(Stories story_list, int strsize)
{
     /* This function is a menu function for viewing storylines */
    int h;
    printf(" ID          Name\n");
    for(h=0; h < strsize; h++) {
        set_color(96); printf("[%d]", story_list[h].story_ID);
        esc_color(); printf(" %s\n", story_list[h].story_name);
    }
    printf("\n");
}
void set_hero_choice_list(MoveSets set_list, int setsize, Heroes hero_list, int h_size)
{
     /* This function is a menu function for viewing names of heroes with assigned moves */
    int h, h_index;
    printf(" ID          Name\n");
    for(h = 0; h < setsize; h++) {
        printf("[%d] ", set_list[h].heroID);
        h_index = get_heroindex(hero_list, set_list[h].heroID, h_size);
        printf("%s\n",hero_list[h_index].hero_name);
    }
}
//Delete functions
void delete_storyline(Stories story_list, int *s_size)
{
    FILE *story_delete;
    story_delete = fopen("storyline.txt","wt");
    int s_id, s_index, i;

    if (story_delete != NULL) {
        do {
            printf("Enter ID of story to be removed : "); scanf("%d", &s_id);
            s_index = get_storyIndex(story_list, s_id, *s_size);
        } while (s_index == -1);

        for (i = 0; i < *s_size - 1; i++) {
            if (i >= s_index) {
                story_list[i] = story_list[i + 1];
                print_storyline(story_list[i], story_delete);
            } else
                print_storyline(story_list[i], story_delete);
        }
        (*s_size)--;
        fclose(story_delete);
        printf("Storyline of ID %d has been deleted.\n", s_id);
    } else printf("ERROR accessing file.\n");
}
void delete_story_hero(Stories story_list, int deletedID, int *s_size)
{
    FILE *story_delete;
    story_delete = fopen("storyline.txt","w");
    int s_index = 0, i;

    if (story_delete != NULL) {
        for (i = 0; i < *s_size; i++) {
            if(story_list[i].main_hero == deletedID)
                s_index = i;
            if (i >= s_index) {
                story_list[i] = story_list[i + 1];
                print_storyline(story_list[i], story_delete);
            } else
                print_storyline(story_list[i], story_delete);
        }
        (*s_size)--;
        fclose(story_delete);
        printf("Storyline/s with main hero %d has been deleted.\n",deletedID);
    } else printf("ERROR accessing file.\n");
}
void delete_story_enemy(Stories story_list, int deletedID, int s_size, MoveSets set_list, int setsize)
{
    FILE *story_delete;
    story_delete = fopen("storyline.txt","w");
    int i, j;

    if (story_delete != NULL) {
        for (i = 0; i < s_size; i++) {
            for(j=0; j < story_list[i].enemy_list[j]; j++) {
                if (story_list[i].enemy_list[j] == deletedID) {
                    printf("Deleted hero ID is found in enemy list.\n");
                    generate_enemy(&story_list[i], j, deletedID, set_list, setsize);
                }
            }
            print_storyline(story_list[i], story_delete);
        }
        fclose(story_delete);
    } else printf("ERROR accessing file.\n");
}
void delete_hero(Heroes hero_list, int *defsize, MoveSets set_list, int setsize, Stories story_list, int strsize)
{
    system("cls");
    FILE *character_remove;
    character_remove = fopen("character.txt", "wt");
    int c_id, c_index, i;
    if (character_remove != NULL) {
        hero_choice_list(hero_list, *defsize);
        do {
            printf("Enter ID of hero to be eliminated: "); scanf("%d",&c_id);
            c_index = get_heroindex(hero_list,c_id, *defsize);
        } while (c_index == -1);

        for(i=0; i< *defsize-1; i++) {
            if (i >= c_index) {
                hero_list[i] = hero_list[i + 1];
                print_hero(hero_list[i], character_remove);
            } else
                print_hero(hero_list[i], character_remove);
        }
        (*defsize)--;
        fclose(character_remove);
        // Remove assigned moves
        delete_gameset(set_list, setsize, c_id);

        int story_hero, story_enemy; //Gets the index of deleted character in Storyline (As hero/As enemy)
        
        // Remove it in storylines where it is a main hero or villain
        story_hero = story_hero_exists(story_list, c_id, strsize);
        story_enemy = story_enemy_exists(story_list, c_id, strsize);

        if (story_hero != -1)
            delete_story_hero(story_list, c_id, &strsize);
        else
            printf("Character not a superhero in a story.\n");
        if (story_enemy != -1)
            delete_story_enemy(story_list, c_id, strsize, set_list, setsize);
        else
            printf("Character not an enemy in a story.\n");

    } else printf("ERROR opening file.\n");
}
void delete_gameset(MoveSets set_list, int *setsize, int deleted_heroID)
{
    /* This function accepts a hero ID and removes its assigned moves */
    printf("Entered the function! DGS\n");
    FILE *gameset_remove;
    gameset_remove = fopen("gameset.txt", "wt");
    int i, g_index = get_gamesetIndex(set_list, deleted_heroID, *setsize);

    if(gameset_remove != NULL) {
        for(i=0; i < *setsize-1; i++)
        {
            if (i >= g_index) {
                set_list[i] = set_list[i + 1];
                print_gameset(set_list[i], gameset_remove);
            } else
                print_gameset(set_list[i], gameset_remove);
        }
        (*setsize)--;
        fclose(gameset_remove);
        printf("The moves assigned to this hero has been deleted.\n");
    } else
        printf("ERROR accessing file.\n");
}
void delete_moves(Moves move_list, int  *m_size, Heroes hero_list, int *defsize, MoveSets set_list, int *setsize)
{
    /* This function deletes a move */
    system("cls");
    FILE *move_delete;
    move_delete = fopen("moves.txt","wt");
    int m_id, m_index, i;

    if (move_delete != NULL) {
        int move_case = 1, elem = -1;
        move_choice_list(move_list, *m_size, move_case, elem);
        do {
            printf("Enter ID of move to be removed: "); scanf("%d", &m_id);
            m_index = get_moveIndex(move_list, m_id, *m_size);
        } while (m_index == -1);

        for (i = 0; i < *m_size - 1; i++) {
            if (i >= m_index) {
                move_list[i] = move_list[i + 1];
                print_move(move_list[i], move_delete);
            } else
                print_move(move_list[i], move_delete);
        }
        (*m_size)--;
        printf("Move of ID %d has been deleted.\n",m_id);
        fclose(move_delete);
        // Generate a random move if deleted move is assigned to a hero
        check_move(hero_list, *defsize, move_list, *m_size, m_id, set_list, *setsize);
    } else printf("ERROR accessing file.\n");
}
//Print to file functions
void print_hero(Hero hero,  FILE *character_edit)
{
    fprintf(character_edit, "%d\n", hero.element);
    fprintf(character_edit, "%s\n", hero.hero_name);
    fprintf(character_edit,"%s\n",hero.stage_name);
    fprintf(character_edit, "%d\n", hero.id);
    fprintf(character_edit,"%d\n", hero.health);
    fprintf(character_edit,"%d\n", hero.power);
    fprintf(character_edit,"%d %d\n", hero.currHP, hero.curPP);
}
void print_storyline(Story story, FILE *storyline_edit)
{
    int i;
    fprintf(storyline_edit, "%d\n", story.story_ID);
    fprintf(storyline_edit, "%s\n", story.story_name);
    fprintf(storyline_edit,"%s\n",story.description);
    fprintf(storyline_edit, "%d\n", story.main_hero);
    fprintf(storyline_edit,"%d\n", story.size);
    for(i=0; i < story.size; i++)
        fprintf(storyline_edit,"%d ", story.enemy_list[i]);
    fprintf(storyline_edit, "\n");

}
void print_gameset(Set gameset, FILE *gameset_edit)
{
    fprintf(gameset_edit, "%d\n", gameset.heroID);
    fprintf(gameset_edit, "%d %d %d\n", gameset.moveID[0], gameset.moveID[1], gameset.moveID[2]);
}
void print_move(Move move, FILE *move_edit)
{
    fprintf(move_edit, "%d\n", move.move_elem);
    fprintf(move_edit, "%s\n",move.move_name);
    fprintf(move_edit,"%s\n",move.description);
    fprintf(move_edit,"%d\n", move.move_id);
    fprintf(move_edit,"%d %d\n",move.move_pp, move.move_type);
    fprintf(move_edit,"%d %d\n",move.move_min,move.move_max);
}
//Rewrite Text File functions
void update_herolist(Heroes hero_list, int defsize, int h_case, int h_index, String replacement)
{
    FILE *character_edit;
    character_edit = fopen("character.txt", "wt");

    int i, replace_int = 0;

    if(h_case == 1 || h_case == 3 || h_case == 4)
        replace_int = atoi(replacement);

    for(i=0; i< defsize;i++) {
        if(i == h_index && h_case == 1)
            hero_list[i].element = replace_int;
        if(i == h_index && h_case == 2)
            strcpy(hero_list[i].hero_name, replacement);
        if(i == h_index && h_case == 3) {
            hero_list[i].health = replace_int;
            hero_list[i].currHP = replace_int;
        }
        if(i == h_index && h_case == 4) {
            hero_list[i].power = replace_int;
            hero_list[i].curPP = replace_int;
        }
        if(i == h_index && h_case == 5)
            strcpy(hero_list[i].stage_name, replacement);

        print_hero(hero_list[i], character_edit);
    }
    fclose(character_edit);
    printf("\nHero Updated!\n");
}
void update_movelist(Moves move_list, int m_size, int m_case, int m_index, String replacement)
{
    FILE *moves_edit;
    moves_edit = fopen("moves.txt", "wt");

    int i, replace_int = 0;

    if (m_case == 1 || m_case == 3 || m_case == 4 || m_case == 6 || m_case == 7)
        replace_int = atoi(replacement);
    for (i = 0; i < m_size; i++) {

        if (i == m_index && m_case == 1)
            move_list[i].move_elem = replace_int;
        if (i == m_index && m_case == 3)
            move_list[i].move_pp = replace_int;
        if (i == m_index && m_case == 4)
            move_list[i].move_type = replace_int;
        if (i == m_index && m_case == 6)
            move_list[i].move_min = replace_int;
        if (i == m_index && m_case == 7)
            move_list[i].move_max = replace_int;
        if (i == m_index && m_case == 2)
            strcpy(move_list[i].move_name, replacement);
        if (i == m_index && m_case == 5)
            strcpy(move_list[i].description, replacement);
        print_move(move_list[i], moves_edit);
    }
    fclose(moves_edit);
    printf("\nSuperpower Updated!\n");
}
void update_storylist(Stories story_list, int strsize, int s_case, int s_index, String replacement)
{
    FILE *story_edit;
    story_edit = fopen("storyline.txt", "wt");

    int i,j, replace_int = 0;

    if (s_case == 3 || s_case == 4 || s_case == 5)
        replace_int = atoi(replacement);
    for (i = 0; i < strsize; i++) {

        if (i == s_index && s_case == 1)
            strcpy(story_list[i].story_name, replacement);
        if (i == s_index && s_case == 2)
            strcpy(story_list[i].description, replacement);
        if (i == s_index && s_case == 3)
            story_list[i].size = replace_int;
        if (i == s_index && s_case == 4)
           story_list[i].main_hero = replace_int;
        if (i == s_index && s_case == 5) {
            for(j=0; j<story_list[i].size; j++) {
                if(story_list[i].enemy_list[j] == story_list[i].main_hero)
                    story_list[i].enemy_list[j] = replace_int;
            }
        }

        print_storyline(story_list[i], story_edit);
    }
    fclose(story_edit);
    printf("\n Storyline Updated!\n");
}
//Modify a Structure Member functions
void edit_hero(Heroes hero_list, int defsize, MoveSets set_list, int setsize)
{
    system("cls");
    print_header("MODIFY HEROES");
    int e_choice, h_id, h_index;
    String edit_elem;
    printf("What do you want to edit?\n");
    set_color(96); printf("[1]"); esc_color(); printf(" Elemental Affinity\n");
    set_color(96); printf("[2]"); esc_color(); printf(" Hero Name\n");
    set_color(96); printf("[3]"); esc_color(); printf(" Hero HP\n");
    set_color(96); printf("[4]"); esc_color(); printf(" Hero Power\n");
    set_color(96); printf("[5]"); esc_color(); printf(" Stage Name\n");
    do { printf("Choice: "); scanf("%d", &e_choice); } while(e_choice<1 || e_choice>5);

    hero_choice_list(hero_list, defsize);
    do {
        printf("Please enter Hero ID: "); scanf("%d",&h_id);
        h_index = get_heroindex(hero_list, h_id, defsize);
    } while(h_index == -1);

    switch (e_choice){
        case 1:
            do {
                printf("Enter element: "); scanf("%s", edit_elem);
            } while(atoi(edit_elem) < 0 || atoi(edit_elem) > 4);

            update_herolist(hero_list,defsize,e_choice,h_index,edit_elem);
            // Check if the hero has an associated moves
            int set_valid = get_gamesetIndex(set_list, h_id, setsize);
            // Remove associated moves
            if(set_valid != -1)
                delete_gameset(set_list, &setsize, h_id);

            break;
        case 2:
            printf("Enter new name of hero: "); scanf(" ");
            do{
                fgets(edit_elem, sizeof(edit_elem),stdin);
                if(strlen(edit_elem)>100)
                    printf("Hero name too long! Enter again.\n");
            }while(strlen(edit_elem) > 100);
            edit_elem[strlen(edit_elem)-1] = '\0';
            update_herolist(hero_list,defsize,e_choice,h_index,edit_elem);

            break;
        case 3:
            do {
                printf("Enter health: "); scanf("%s", edit_elem);
            } while(atoi(edit_elem) < 50 || atoi(edit_elem) > 500);

            update_herolist(hero_list,defsize,e_choice,h_index,edit_elem);
            break;
        case 4:
            do {
                printf("Enter power: "); scanf("%s", edit_elem);
            } while(atoi(edit_elem) < 10 || atoi(edit_elem) > 40);

            update_herolist(hero_list,defsize,e_choice,h_index,edit_elem);
            break;
        case 5:
            printf("Enter Stage Name: "); scanf(" ");
            do{
                fgets(edit_elem, sizeof(edit_elem),stdin);
                if(strlen(edit_elem)>100)
                    printf("Stage name too long! Enter again.\n");
            }while(strlen(edit_elem) > 100);
            edit_elem[strlen(edit_elem)-1] = '\0';
            update_herolist(hero_list,defsize,e_choice,h_index,edit_elem);
            break;
        default: printf("Not applicable.\n"); break;
    }
}
void edit_moves(Moves move_list, int m_size, Heroes hero_list, int defsize, MoveSets set_list, int setsize)
{
        system("cls");
        print_header("MODIFY POWERS");
        int m_choice, m_id, m_index;
        char eh_choice; String edit_elem;
    
    printf("What do you want to edit?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Elemental Affinity\n");
        set_color(96); printf("[2]"); esc_color(); printf(" Superpower Name\n");
        set_color(96); printf("[3]"); esc_color(); printf(" PP Requirement\n");
        set_color(96); printf("[4]"); esc_color(); printf(" Power Type\n");
        set_color(96); printf("[5]"); esc_color(); printf(" Superpower Description\n");
        set_color(96); printf("[6]"); esc_color(); printf(" Min Damage\n");
        set_color(96); printf("[7]"); esc_color(); printf(" Max Damage\n");
    
        do { printf("Choice: "); scanf("%d", &m_choice); } while(m_choice< 1 || m_choice>7);
        int move_case = 3, elem = 0;
        move_choice_list(move_list, m_size, move_case, elem);
        do {
            printf("Please enter Move ID: "); scanf("%d",&m_id);
            m_index = get_moveIndex(move_list, m_id, m_size);
        } while(m_index == -1);

        switch (m_choice) {
            case 1:
            do {
                printf("Enter element: "); scanf("%s", edit_elem);
            } while(atoi(edit_elem) < 0 || atoi(edit_elem) > 4);

            check_move(hero_list, defsize, move_list, m_size, m_id, set_list, setsize);
            update_movelist(move_list,m_size,m_choice,m_index,edit_elem);

            printf("Go back to edit superpower menu?(y/n): ");scanf(" "); scanf("%c", &eh_choice);
            if(eh_choice == 'y')
                edit_moves(move_list, m_size, hero_list, defsize, set_list, setsize);
            break;
            case 2:
                 printf("Enter new name of Superpower: "); scanf(" ");
                 do{
                    fgets(edit_elem, sizeof(edit_elem),stdin);
                    if(strlen(edit_elem)>100)
                        printf("Superpower name too long! Enter again.\n");
                 }while(strlen(edit_elem) > 100);

            edit_elem[strlen(edit_elem)-1] = '\0';

            update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
            break;
            case 3:
                do {
                    printf("Minimum 10. Maximum 40.\n");
                    printf("Enter PP requirement: "); scanf("%s", edit_elem);
                } while(atoi(edit_elem) < 20 || atoi(edit_elem) > 100);

                update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
                break;
            case 4:
            do {
                printf("1-Light  2-Medium 3-Heavy");
                printf("Enter power type: "); scanf("%s", edit_elem);
            } while(atoi(edit_elem) < 1 || atoi(edit_elem) > 3);

            update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
            break;
            case 5:
                printf("Enter new description of Superpower: "); scanf(" ");
                do{
                    fgets(edit_elem, sizeof(edit_elem),stdin);
                    if(strlen(edit_elem)>100)
                        printf("Description too long! Enter again.\n");
                }while(strlen(edit_elem) > 100);
                edit_elem[strlen(edit_elem)-1] = '\0';

                update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
            break;
            case 6:
                do {
                    printf("Maximum is 180. Minimum is 20. Difference of Min. and Max. should not exceed 60.\n");
                    printf("Enter new minimum damage: "); scanf("%s", edit_elem);
                } while(atoi(edit_elem) < 20 || atoi(edit_elem) > 180 || move_list[m_index].move_max-atoi(edit_elem)>60 ||
                atoi(edit_elem) == move_list[m_index].move_max || atoi(edit_elem) > move_list[m_index].move_max);

                update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
                break;
            case 7:
                do {
                    printf("Maximum is 180. Minimum is 20. Difference of Min. and Max. should not exceed 60.\n");
                    printf("Enter new maximum damage: "); scanf("%s",edit_elem);
                } while(atoi(edit_elem) < 20 || atoi(edit_elem) > 180 || atoi(edit_elem)-move_list[m_index].move_min>60 ||
                       atoi(edit_elem) == move_list[m_index].move_min || atoi(edit_elem) < move_list[m_index].move_min);

                update_movelist(move_list,m_size,m_choice,m_index,edit_elem);
                break;
            default: printf("No such choice silly human!\n"); break;
        }
};
void edit_storyline(Stories story_list, int strsize, MoveSets set_list, int setsize)
{
    system("cls");
    print_header("MODIFY STORIES");
    int story_id, s_choice, s_index, gs_index, e_index = 0, max, old_size, i; String replacement, new_enemy;
   
    printf("What would you like to edit in the storyline?\n");
    set_color(96); printf("[1]"); esc_color(); printf(" Story Name\n");
    set_color(96); printf("[2]"); esc_color(); printf(" Story Description\n");
    set_color(96); printf("[3]"); esc_color(); printf(" Number of Stages\n");
    set_color(96); printf("[4]"); esc_color(); printf(" Main Hero\n");
    
    do { printf("Choice: "); scanf("%d", &s_choice); } while(s_choice < 0 || s_choice > 4);

    do {
        printf("Enter a Storyline ID: ");
        scanf("%d", &story_id);
        s_index = get_storyIndex(story_list, story_id, strsize);
    } while(s_index == -1);

    switch(s_choice) {
        case 1:
            do {
                printf("Enter a new name: ");
                scanf(" "); fgets(replacement, sizeof(replacement), stdin);
                if (strlen(replacement) > 100)
                    printf("Too long!\n");
            } while (strlen(replacement) > 100);
            replacement[strlen(replacement) - 1] = '\0';

            update_storylist(story_list, strsize, s_choice, s_index, replacement);
            break;
        case 2:
            do {
                printf("Enter a new description: "); scanf(" ");
                fgets(replacement, sizeof(replacement), stdin);
                if (strlen(replacement) > 100)
                    printf("Too long!\n");
            } while (strlen(replacement) > 100);
            replacement[strlen(replacement) - 1] = '\0';

            update_storylist(story_list, strsize, s_choice, s_index, replacement);
            break;
        case 3:
            do {
                max = setsize - 1; // # of possible stages is the # of heroes with 3 assigned moves, exclude protagonist
                printf("Minimum is 3. Maximum is %d.\n", max);
                printf("Enter new number of stages");
                fgets(replacement, sizeof(replacement), stdin);

                if (atoi(replacement) > max)
                    printf("Too big!\n");
                else if (atoi(replacement) < 3)
                    printf("Too small!\n");
            } while (atoi(replacement) < 3 || atoi(replacement) > setsize);
            replacement[strlen(replacement) - 1] = '\0';
            old_size = story_list[s_index].size;
            update_storylist(story_list, strsize, s_choice, s_index, replacement);

            int diff, valid;
            if(atoi(replacement) > old_size) {
                diff = atoi(replacement) - old_size;
                while(diff != 0) {
                    do {
                        printf("Enter a character ID to add: "); scanf("%s", new_enemy);
                        e_index = get_gamesetIndex(set_list, atoi(new_enemy), setsize);
                        valid = enemy_repeat(story_list[s_index],atoi(new_enemy));

                        if(atoi(new_enemy) == story_list[s_index].main_hero)
                            printf("That's the same ID as your main hero. Enter again.\n");
                        else if(e_index == -1)
                            printf("That character doesn't have any assigned moves. Choose another.\n");
                        else if (valid == -1)
                            printf("This enemy is already an enemy. Pick again.\n");
                    } while(e_index == -1 || atoi(new_enemy) == atoi(replacement));
                    (*story_list[s_index].enemy_list)++;
                    diff--;
                }
            } else {
                diff = old_size-atoi(replacement);
                while (diff != 0) {
                    (*story_list[s_index].enemy_list)--;
                    diff--;
                }
            }

            break;
        case 4:
            do {
                printf("Enter ID of new main hero: ");
                fgets(replacement, sizeof(replacement), stdin);
                gs_index = get_gamesetIndex(set_list, atoi(replacement), setsize);
                if (gs_index == -1)
                    printf("Not a valid hero ID!\n");
            } while (gs_index == -1);
            replacement[strlen(replacement)-1] = '\0';
            update_storylist(story_list, strsize, s_choice, s_index, replacement); //Change the protagonist ID

            // Check if protagonist ID is the same as enemy_list[i] ID
            for (i = 0; i < story_list[s_index].size; i++) {
                if(story_list[s_index].enemy_list[i] == atoi(replacement)) {
                    printf("Oh no, Your main hero is also an enemy in this story!\n");
                    do {
                        printf("Enter a character ID to replace: "); scanf("%s", new_enemy);
                        e_index = get_gamesetIndex(set_list, atoi(new_enemy), setsize);

                        if(atoi(new_enemy) == atoi(replacement))
                            printf("That's the same ID as your main hero. Enter again.\n");
                        else if(e_index == -1)
                            printf("That character doesn't have any assigned moves. Choose another.\n");

                    } while(e_index == -1 || atoi(new_enemy) == atoi(replacement));
                    s_choice = 5;
                    update_storylist(story_list, strsize, s_choice, s_index, new_enemy);
                }
            }
            break;
        default:printf("No such choice!\n");
    }
}
// Configuration Menu functions
void character_menu(Heroes hero_list, int *defsize, MoveSets set_list, int *setsize, Moves  move_list, int *m_size, Stories story_list, int strsize)
{
    int c_select, running = 1;
    while(running) {
        system("cls");
        print_header("HERO UNIVERSE");
        printf("What would you like to do in the Hero Universe?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Add a Hero\n");
        set_color(96); printf("[2]"); esc_color(); printf(" Modify a Hero\n");
        set_color(96); printf("[3]"); esc_color(); printf(" View a Hero\n");
        set_color(96); printf("[4]"); esc_color(); printf(" Delete a Hero\n");
        printf("Choice: ");
        scanf("%d", &c_select);

        switch (c_select) {
            case 1:
                while (running) {
                    add_hero(hero_list, defsize);
                    do {
                        printf("\n\nAdd hero again?\n[1] Yes\n[0] No\nChoice: ");
                        scanf("%d", &running);
                    } while (running < 0 || running > 1);
                }
                break;
            case 2:
                while (running) {
                    edit_hero(hero_list, *defsize, set_list, *setsize);
                    do {
                        printf("\n\nModify hero again?\n[1] Yes\n[0] No\nChoice: ");
                        scanf("%d", &running);
                    } while (running < 0 || running > 1);
                }
                break;
            case 3:
                while (running) {
                    view_hero(hero_list, *defsize);
                    do {
                        printf("\n\nView again?\n[1] Yes\n[0] No\nChoice: ");
                        scanf("%d", &running);
                    } while (running < 0 || running > 1);
                }
                break;
            case 4:
                while (running) {
                    delete_hero(hero_list, defsize, set_list, *setsize, story_list, strsize);
                    do {
                        printf("\n\nDelete hero again?\n[1] Yes\n[0] No\nChoice: ");
                        scanf("%d", &running);
                    } while (running < 0 || running > 1);
                }
                break;
            default:printf("No such choice!\n");
        }

        printf("\nGo back to hero menu again?\n[1] Yes\n[0] No\n");
        do {
            printf("Choice: "); scanf("%d", &running);
        } while (running<0 || running>1);
    }
}
void story_menu(Stories story_list, int story_size, Heroes hero_list, int defsize, MoveSets set_list, int setsize, Moves move_list, int move_size)
{
int running = 1, s_select;
while(running)
{
    system("cls");
    print_header("STORYLINE MENU");
    printf("Time to play God and create your own realities.\n");
    set_color(96); printf("[1]"); esc_color(); printf(" Add a Storyline\n");
    set_color(96); printf("[2]"); esc_color(); printf(" View a Storyline\n");
    set_color(96); printf("[3]"); esc_color(); printf(" Modify a Storyline\n");
    set_color(96); printf("[4]"); esc_color(); printf(" Delete a Storyline\n");

    do {
        printf("Choice: "); scanf("%d", &s_select);
    } while (s_select < 1 || s_select > 4);

    switch(s_select)
    {
        case 1: add_storyline(story_list, &story_size, hero_list, defsize,set_list, setsize, move_list, move_size); break;
        case 2: view_storyline(story_list, story_size, hero_list, defsize); break;
        case 3: edit_storyline(story_list, story_size, set_list, setsize); break;
        case 4: delete_storyline(story_list, &story_size); break;
        default: printf("Not a valid choice for storyline menu.\n");
    }

    printf("Go back to storyline menu again?\n[1] Yes\n[0] No\n");
    do {
        printf("Choice: "); scanf("%d", &running);
    } while(running < 0 || running > 1);
}
}
void moves_menu(Moves move_list, int move_size, Heroes hero_list, int defsize, MoveSets set_list, int setsize)
{
    int m_select, heroID, running = 1;

    while (running) {
        system("cls");
        print_header("SUPERPOWER MENU");
        printf("Time for fun. What would you like to do with the moves?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Add a Superpower\n");
        set_color(96); printf("[2]"); esc_color(); printf(" View a Superpower\n");
        set_color(96); printf("[3]"); esc_color(); printf(" Modify a Superpower\n");
        set_color(96); printf("[4]"); esc_color(); printf(" Delete a Superpower\n");
        set_color(96); printf("[5]"); esc_color(); printf(" Set Superpowers to a Hero\n");
        do {
            printf("Choice: ");
            scanf("%d", &m_select);
        } while (m_select < 1 || m_select > 6);

        switch (m_select) {
            case 1:
                add_moves(move_list, &move_size, hero_list, defsize, set_list, setsize);
                break;
            case 2:
                view_moves(move_list, move_size);
                break;
            case 3:
                edit_moves(move_list, move_size, hero_list, defsize, set_list, setsize);
                break;
            case 4:
                delete_moves(move_list, &move_size, hero_list, &defsize, set_list, &setsize);
                break;
            case 5:
                do {
                    hero_choice_list(hero_list, defsize);
                    printf("Enter a Hero ID: ");
                    scanf("%d", &heroID);
                } while (get_heroindex(hero_list, heroID, defsize) == -1);
                printf("\n");
                set_moves(set_list, hero_list, move_list, heroID, move_size, &setsize, defsize);
                break;
            default:printf("No such choice!\n");
        }
        printf("Go back to moves menu again?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Yes\n");
        set_color(96); printf("[0]"); esc_color(); printf(" No\n");
        do {
            printf("Choice: ");scanf("%d", &running);
        } while (running<0 || running>1);
    }
}
void storyline_menu(Stories story_list, int str_size, MoveSets set_list, int setsize, Heroes hero_list, int h_size,
        Moves move_list, int m_size, Backgrounds background_list, int bg_size)
{
    int running = 1;
    while (running) {
        system("cls");
        print_header("STORYLINE MENU");
        int story_index, storyID;
        printf("Pick a storyline: \n");
        storyline_choice_list(story_list, str_size);
        do {
            printf("Choice: ");
            scanf("%d", &storyID);
            story_index = get_storyIndex(story_list, storyID, str_size);
            if (story_index == -1)
                printf("No such story. Pick again!\n");
        } while (story_index == -1);

        begin_battle(story_list[story_index], set_list, setsize, hero_list, h_size, move_list, m_size, background_list,
                     bg_size);

        printf("Play Again?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Yes\n");
        set_color(96); printf("[0]"); esc_color(); printf(" No\n");
        do {
            printf("Choice: ");scanf("%d", &running);
        } while (running < 0 || running > 1);
    }

}
void freeplay_menu(MoveSets set_list, int setsize, Heroes hero_list, int h_size, Moves move_list, int m_size,
        Backgrounds background_list, int bg_size)
{
    int superheroID, set_hero_index, hero_index, enemyID, set_enemy_index, enemy_index;

    printf("Pick a superhero: \n");
    set_hero_choice_list(set_list, setsize, hero_list, h_size);
    do {
        printf("\nChoice: "); scanf("%d", &superheroID);
        set_hero_index = get_gamesetIndex(set_list, superheroID, setsize);
        hero_index = get_heroindex(hero_list, superheroID, h_size);
        if(set_hero_index == -1)
            printf("Not a valid hero! Pick again.\n");

    } while(set_hero_index == -1);

    printf("Pick your enemy: \n");
    set_hero_choice_list(set_list, setsize, hero_list, h_size);
    do {
        printf("\nChoice: "); scanf("%d", &enemyID);
        set_enemy_index = get_gamesetIndex(set_list, enemyID, setsize);

        if(set_enemy_index == -1)
            printf("Not a valid hero! Pick again.\n");
        else if (set_enemy_index == set_hero_index)
            printf("That's your superhero already. Pick again.\n");
    } while(set_hero_index == set_enemy_index || set_enemy_index == -1);

    int enemy_score = 0, hero_score = 0, running = 3, round = -1;
    // Initialize PP to Zero
    enemy_index = get_heroindex(hero_list, enemyID, h_size);
    hero_list[enemy_index].currHP = hero_list[enemy_index].health;
    hero_list[hero_index].curPP = hero_list[hero_index].health;
    hero_list[enemy_index].curPP = 0;
    hero_list[hero_index].curPP = 0;
    while(running == 3)
    {
        running = proceed_round(round, &hero_list[hero_index], &hero_list[enemy_index], set_list[set_hero_index],
                                set_list[set_enemy_index], move_list, m_size, background_list, bg_size); // Return 1 if there's a winner and 0 if it ends
        system("pause");
	   // printf("return value %d\n", running);
        if (running == 1) 
            enemy_score++;
        else if(running == 2)
            hero_score++;
    }

    if (enemy_score > hero_score) {
        system("cls"); set_color(93);
        view_backgroundbyElement(background_list, bg_size, 5);
        esc_color();
    }
    else if(enemy_score < hero_score) {
        system("cls"); set_color(36);
        view_backgroundbyElement(background_list, bg_size, 6);
        esc_color();
    }
    system("pause");
}
void gameplay_menu(Stories story_list, int str_size, MoveSets set_list, int setsize, Heroes hero_list, int h_size,
        Moves move_list, int m_size, Backgrounds background_list, int bg_size)
{
    system("cls");
    print_header("GAMEPLAY MENU");
    int p_choice;
    printf("Choose a game mode:\n");
	set_color(93); printf("[1] "); esc_color(); printf("Storyline\n");
	set_color(93); printf("[2] "); esc_color(); printf("Freeplay\n");
    do {
        printf("Choice: "); scanf("%d", &p_choice);
    } while(p_choice < 1 || p_choice > 2);

    if(p_choice == 1)
        storyline_menu(story_list, str_size, set_list, setsize, hero_list, h_size, move_list, m_size, background_list, bg_size);
    else
        freeplay_menu(set_list, setsize, hero_list, h_size, move_list, m_size, background_list, bg_size);
}
int compute_damage(Move attack, Hero superhero, Hero enemy)
{
    float damage, additional_dmg = 1;
    float damage_chance = 50, dodge_chance = 50;

    if (attack.move_type == 1) {
        dodge_chance = sys_rand(100); // Generate % chance of success
        damage_chance = sys_rand(100); // Light Attack : Generate 5-15% damage to player

        if(damage_chance < 5)
            damage_chance = 5;
        else if (damage_chance >= 15)
            damage_chance = 15;

        if (dodge_chance <= 25) {
            dodge_chance = 100;
            printf("Enemy Dodged!\n");
        }
    } else if (attack.move_type == 2) {
        dodge_chance = sys_rand(100); // Generate % chance of success
        damage_chance = sys_rand(100); //  Medium Attack: Generate 20-30% damage to player

        if(damage_chance < 20)
            damage_chance = 20;
        else if (damage_chance > 30)
            damage_chance = 30;
        if (dodge_chance <= 10) {
            dodge_chance = 100;
            printf("Enemy Dodged!\n");
        }
    } else if (attack.move_type == 3) {
        dodge_chance = 0; // % chance of success
        damage_chance = sys_rand(100); // Heavy Attack: Generate 40-50% damage to player
        if(damage_chance < 40)
            damage_chance = 40;
        else if (damage_chance > 50)
            damage_chance = 50;
    }

    if(superhero.element == enemy.element)
        additional_dmg = 0.75; // If same element, enemy will get .75 of damage
    else if(superhero.element == 1 || enemy.element == 4)
        additional_dmg = 1.5; // Water v Earth
    else if(superhero.element == 4 || enemy.element == 1)
        additional_dmg = 1.5; // Earth v Water
    else if(superhero.element == 2 || enemy.element == 3)
        additional_dmg = 1.5; // Fire v Air
    else if(superhero.element == 3 || enemy.element == 2)
        additional_dmg = 1.5; // Air v Fire

        //Generate a move damage
    damage = (1-(dodge_chance/100)) *  (additional_dmg) * ((damage_chance/100) * enemy.health);


    return (int)damage ;
}
int compute_pp_cost(Move attack, Hero character)
{
    float pp_chance;
    if (attack.move_type == 1) {
        pp_chance = sys_rand(100); // Light attack consumes 10 to 20% of the maximum PP
        if (pp_chance < 10)
            pp_chance = 10;
        else if (pp_chance > 20)
            pp_chance = 20;
        return (int) (character.power * (pp_chance/100));
    } else if (attack.move_type == 2) {
        pp_chance = sys_rand(100); // Medium attack consumes 30 to 50% of the maximum PP
        if (pp_chance < 30)
            pp_chance = 30;
        else if (pp_chance > 50)
        pp_chance = 50;
        return (int) (character.power * (pp_chance/100));
    } else  {
        pp_chance = sys_rand(100); // Heavy attack consumes 30 to 50% of the maximum PP
        if (pp_chance < 60)
            pp_chance = 60;
        else if (pp_chance > 80)
            pp_chance = 80;
        return (int) (character.power * (pp_chance/100));
    }

}
void computer_turn(Hero *superhero, Hero *enemy, Set enemy_moves, Moves move_list, int m_size)
{
    int set_move_index, enemy_move_index, enough_pp = 0, pp_cost, damage = 0;

    do {
        set_move_index = sys_rand(4) - 1;
        enemy_move_index = get_moveIndex(move_list, enemy_moves.moveID[set_move_index], m_size);

        if (set_move_index < 3) { // If computer chooses a move, there should be enough PP
            enough_pp = move_list[enemy_move_index].move_pp;
            if(enough_pp > enemy->curPP)
                enough_pp = -1;
        } else
            enough_pp = 0; // No PP required for Charge move
    } while(enough_pp == -1);

    if (set_move_index < 3) {
        damage = compute_damage(move_list[enemy_move_index], *superhero, *enemy);
        pp_cost = compute_pp_cost(move_list[enemy_move_index], *superhero);
        enemy->curPP -= pp_cost;
        enemy->curPP -= move_list[enemy_move_index].move_pp;
        enemy->curPP += 2; printf("Enemy gained 2 PP.\n");
        if (enemy->curPP < 0)
            enemy->curPP = 0;
        else if (enemy->curPP > enemy->power)
            enemy->curPP = enemy->power;
        if(damage != 0)
          printf("%s used %s! Damage dealt is %d!\n", enemy->hero_name, move_list[enemy_move_index].move_name, damage);
        else
          printf("You Dodged!\n");
    } else {
        enemy->curPP += 3;
        printf("%s used %s! Enemy gains 3 PP!\n", enemy->hero_name, "Charge");
    }
    superhero->currHP -= damage;
    if(superhero->currHP < 0)
        superhero->currHP = 0;
}
void user_turn(Hero *superhero, Hero *enemy, Set superhero_moves, Moves move_list, int m_size)
{
    
    int i, m_index, choice, required_pp;
    printf("Pick a move:\n");
    for(i=0; i < 4; i++) {
        if(i < 3) {
            m_index = get_moveIndex(move_list, superhero_moves.moveID[i], m_size);
            printf("[%d] %s - %s", i+1, move_list[m_index].move_name, move_list[m_index].description);
        } else
            printf("[%d] Charge - Gain 3 PP\n", i+1);
    }
    // Check if enough PP
    do {
        printf("Choice: "); scanf("%d", &choice);
        m_index = get_moveIndex(move_list, superhero_moves.moveID[choice-1], m_size);
        required_pp = move_list[m_index].move_pp;
        if(required_pp > superhero->curPP)
            printf("Not enough PP. Pick another move.\n");
    } while (choice < 1 || choice > 4 || required_pp > superhero->curPP);

    Move attack;
    switch (choice) {
        case 1:
            m_index = get_moveIndex(move_list, superhero_moves.moveID[0], m_size);
            attack = move_list[m_index];
            break;
        case 2:
            m_index = get_moveIndex(move_list, superhero_moves.moveID[1], m_size);
            attack = move_list[m_index];
            break;
        case 3:
            m_index = get_moveIndex(move_list, superhero_moves.moveID[2], m_size);
            attack = move_list[m_index];
            break;
        case 4:
            superhero->curPP += 3;
            if(superhero->curPP < 0)
                superhero->curPP = 0;
            else if (superhero->curPP > superhero->power)
                superhero->curPP = superhero->power;
            break;
        default: printf("No such choice!\n");
    }

    int damage = 0, pp_cost = 0;
    if(choice != 4) {
        damage = compute_damage(attack, *superhero, *enemy); // Returns computed damage
        pp_cost = compute_pp_cost(attack, *superhero);
        superhero->curPP -= pp_cost;
        if(superhero->curPP < 0) // To avoid negative PP
            superhero->curPP = 0;
        else if (superhero->curPP > superhero->power)
            superhero->curPP = superhero->power;
        superhero->curPP += 2;
        printf("You gained 2 PP.\n");
    }

    enemy->currHP -= damage;
    if(enemy->currHP < 0)
        enemy->currHP = 0;

    if (damage != 0)
        printf("You dealt %d damage.\n", damage);
}
int proceed_round(int round, Hero *superhero, Hero *enemy, Set superhero_moves, Set enemy_moves, Moves move_list,
        int m_size, Backgrounds background_list, int bg_size)
{
    system("cls");
    if (round == -1) {
        print_header("FREEPLAY");
        printf("%s\n", enemy->stage_name);
        view_backgroundbyElement(background_list, bg_size, enemy->element);
        print_contenders(*superhero, *enemy);
    }  else {
        printf("Stage %d: %s\n", round, enemy->stage_name);
        view_backgroundbyElement(background_list, bg_size, enemy->element);
        print_contenders(*superhero, *enemy);
    }

    user_turn(superhero, enemy, superhero_moves, move_list, m_size);
    computer_turn(superhero, enemy, enemy_moves, move_list, m_size);

    if (enemy->currHP == 0 || superhero->currHP == 0) {
        if (enemy->currHP > superhero->currHP) {
            superhero->currHP = superhero->health; // Restore full health before next round
            return 1; // Enemy scores this round
        }
        else {
            superhero->currHP = superhero->health; // Restore full health before next round
            return 2; // Hero scores this round
        }
    } else
        return 3; // Repeat Player and AI turns
}
void begin_battle(Story story, MoveSets set_list, int setsize, Heroes hero_list, int h_size, Moves move_list, int m_size,
        Backgrounds background_list, int bg_size)
{
    system("cls");
    int hero_index = get_heroindex(hero_list, story.main_hero, h_size);
    int hero_set_index = get_gamesetIndex(set_list, story.main_hero, setsize);

    int round = 1, running = 1, enemy_index, enemy_set_index;
    enemy_index = get_heroindex(hero_list,story.enemy_list[round-1], h_size);

    int hero_score = 0, enemy_score = 0;
    // Initialize PP to Zero
    hero_list[hero_index].curPP = 0;
    hero_list[enemy_index].curPP = 0;
    while (round <= story.size) {
        // Get enemy data
        enemy_index = get_heroindex(hero_list,story.enemy_list[round-1], h_size);
        enemy_set_index = get_gamesetIndex(set_list, story.enemy_list[round-1], setsize);
        // Set PP to Zero for both Players
        // Start Match
        running = proceed_round(round, &hero_list[hero_index], &hero_list[enemy_index], set_list[hero_set_index],
                set_list[enemy_set_index], move_list, m_size, background_list, bg_size); // Return 1 if there's a winner and 0 if it ends
                system("pause");
        // Match Results
        if (running == 1) {
            enemy_score++;
            round++;
            hero_list[hero_index].curPP = 0;
            enemy_index = get_heroindex(hero_list,story.enemy_list[round-1], h_size);
            hero_list[enemy_index].curPP = 0;

        }
        else if(running == 2) {
            hero_score++;
            round++;
            hero_list[hero_index].curPP = 0;
            enemy_index = get_heroindex(hero_list,story.enemy_list[round-1], h_size);
            hero_list[enemy_index].curPP = 0;
        }
    }
    // Game Conclusion & End Screen
   if (enemy_score > hero_score) {
       system("cls"); set_color(95);
       view_backgroundbyElement(background_list, bg_size, 5); // Game Over ASCII Art
       esc_color();
       printf("\n\t\tEnemy side has won.\n");
   }
   else if(enemy_score < hero_score) {
       system("cls:");
       set_color(93);
       view_backgroundbyElement(background_list, bg_size, 6); // Congratulations Art
       esc_color();
   } else
       set_color(36);
       view_backgroundbyElement(background_list, bg_size, 7); // Draw ASCII Art
       esc_color();

   system("pause");
}
// Get size of Background
Environment read_background(FILE *background)
{
    Environment bg; int i;
    bg.elem_background = 0;
    fscanf(background, "%d ", &bg.elem_background);

     for (i = 0; i < 12; i++) {
         fgets(bg.design[i], sizeof(bg.design[i]), background);
         bg.design[i][strlen(bg.design[i])-2] = '\0';
    }
    return bg;
}
int get_bgsize(Backgrounds background_list)
{
    FILE *bg_size;
    bg_size = fopen("environment.txt", "rt");
    int i = 0;

    if(bg_size != NULL)
    {
        background_list[i] = read_background(bg_size);
        if (background_list[i].elem_background != 0) {
            i++;
            while (!feof(bg_size)) {
                background_list[i] = read_background(bg_size);
                i++;
            }
        } else
            i = 0;
        fclose(bg_size);
    } else
        printf("FILE ERROR");
    printf("Background Size: %d\n", i);
    return  i;
}
//Get size of Game Set functions
Set read_set(FILE *gameset)
{
    Set s; int i;
    fscanf(gameset,"%d ", &s.heroID);
    for(i=0; i<3; i++)
        fscanf(gameset, "%d ", &s.moveID[i]);
    return s;
}
int get_gsize(MoveSets set_list)
{
    FILE *gameset_size;
    gameset_size = fopen("gameset.txt", "rt");
    int i = 0;
    if(gameset_size != NULL)
    {
        set_list[i] = read_set(gameset_size);
        if (set_list[i].heroID != 0) {
            i++;
            while(!feof(gameset_size)) {
            set_list[i] = read_set(gameset_size);
            i++;
            }
       } else
            i = 0;
        fclose(gameset_size);
    } else printf("ERROR accessing file. \n");
    printf("Game Set Size: %d\n", i);
    return i;
}
//Get size of Moves functions
Move read_move(FILE *m_size)
{
    Move m; int len;
    m.move_elem = 0;
    fscanf(m_size, "%d ",&m.move_elem);
    fgets(m.move_name, sizeof(m.move_name), m_size);
    len = (int)strlen(m.move_name);
    if (len > 0 && m.move_name[len-1] == '\n')
        m.move_name[len-1] = '\0';
    fgets(m.description, sizeof(m.description), m_size);
    len = (int)strlen(m.description);
    if (len > 0 && m.description[len-1] == '\n')
        m.move_name[len-1] = '\0';
    fscanf(m_size,"%d%d%d%d%d ",&m.move_id, &m.move_pp,&m.move_type,&m.move_min, &m.move_max);

    return m;
}
int get_msize(Moves move_list)
{
    FILE *m_size;
    m_size = fopen("moves.txt", "rt");
    int i = 0;

    if(m_size != NULL)
    {
        move_list[i] = read_move(m_size);
        if (move_list[i].move_id != 0) {
            i++;
            while (!feof(m_size))
            {
                move_list[i] = read_move(m_size);
                i++;
            }
        } else
            i = 0;
        fclose(m_size);
    } else
        printf("FILE ERROR");
    printf("Moves Size: %d\n", i);
    return  i;
}
//Get size of Heroes functions
Hero read_hero(FILE *h_size)
{
    Hero h; int len;
    h.element = 0;
    fscanf(h_size, "%d ",&h.element);
    fgets(h.hero_name, sizeof(h.hero_name), h_size);
    len = (int)strlen(h.hero_name);
    if (len > 0 && h.hero_name[len-1] == '\n')
        h.hero_name[len-1] = '\0';
    fgets(h.stage_name, sizeof(h.stage_name), h_size);
    len = (int)strlen(h.stage_name);
    if (len > 0 && h.stage_name[len-1] == '\n')
        h.stage_name[len-1] = '\0';
    fscanf(h_size,"%d%d%d%d%d ",&h.id, &h.health,&h.power,&h.currHP,&h.curPP);

    return h;
}
int get_hsize(Heroes hero_list)
{
    FILE *h_size;
    h_size = fopen("character.txt", "rt");
    int i = 0;

    if(h_size != NULL)
    {
        hero_list[i] = read_hero(h_size);
        if (hero_list[i].element!= 0) {
            i++;
            while (!feof(h_size)) {
                hero_list[i] = read_hero(h_size);
                i++;
            }
        } else
            i = 0;
        fclose(h_size);
    } else
        printf("FILE ERROR");
    printf("Hero Size: %d\n", i);
    return  i;
}
//Get size of Storyline functions
Story read_story(FILE *stry_size)
{
    Story s; int len, i;

    fscanf(stry_size, "%d ",&s.story_ID);
    fgets(s.story_name, sizeof(s.story_name), stry_size);
    len = (int)strlen(s.story_name);
    if (len > 0 && s.story_name[len-1] == '\n')
        s.story_name[len-1] = '\0';
    fgets(s.description, sizeof(s.description), stry_size);
    len = (int)strlen(s.description);
    if (len > 0 && s.description[len-1] == '\n')
        s.description[len-1] = '\0';
    fscanf(stry_size,"%d%d ",&s.main_hero,&s.size);
    for(i=0; i < s.size;i++)
    {
         fscanf(stry_size,"%d ",&s.enemy_list[i]);
    }
    return s;
}
int get_strysize(Stories story_list)
{
    FILE *stry_size;
    stry_size = fopen("storyline.txt", "rt");
    int i = 0;

    if(stry_size != NULL)
    {
        story_list[i] = read_story(stry_size);
        if (story_list[i].story_ID != 0) {
            i++;
            while (!feof(stry_size)) {
                story_list[i] = read_story(stry_size);
                i++;
            }
        } else
            i = 0;
        fclose(stry_size);
    } else
        printf("FILE ERROR\n");
    printf("Story Line Size: %d\n", i);
    return i;
}
// UI Functions
void print_header(String word)
{
     /* This function accepts a string and prints it to a header */
    printf("-----------------------------------------\n");
    set_color(93); printf("                %s\n", word);
    esc_color();
    printf("-----------------------------------------\n");
}
void print_contenders(Hero hero, Hero enemy)
{
    /* This function accepts a hero and enemy and print their details.
     * Called in gameplay mode battle functions. */
    printf("+----------------------------------------------------+\n");
    printf("     %s               %s      \n", hero.hero_name, enemy.hero_name);
    printf("         ");element_converter(hero.element);   printf("                          ");
    element_converter(enemy.element);printf("     \n");
    printf("      HP:%d/%d                    HP:%d/%d   \n", hero.currHP, hero.health, enemy.currHP, enemy.health);
    printf("       PP:%d/%d                      PP:%d/%d   \n", hero.curPP, hero.power, enemy.curPP, enemy.power);
    printf("+----------------------------------------------------+\n");
}
void set_color(int color) {
    /* This function accepts an integer to set as color */
    printf("%c[%dm", 0x1B, color);
}
void esc_color() {
    /* This function escapes/ends color */
    printf("\x1B[m");
}
//Main menu function
void main_menu()
{
    Heroes hero_list;
    Moves move_list;
    MoveSets set_list;
    Stories story_list;
    Backgrounds background_list;
    int mode, c_mode, running = 1;
    int defsize = get_hsize(hero_list), move_size = get_msize(move_list);
    int setsize = get_gsize(set_list), story_size = get_strysize(story_list);
    int bg_size = get_bgsize(background_list);

    while (running) {
         system("cls");
	     set_color(93); view_backgroundbyElement(background_list, bg_size, 8); 
	     printf("\n");
		 esc_color(); system("pause");
	     system("cls");
        print_header("MAIN MENU");
        printf("Welcome traveller, which path are you taking:\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Gameplay Path\n");
        set_color(96); printf("[2]"); esc_color(); printf(" Configure Path\n");
        set_color(96); printf("[3]"); esc_color(); printf(" I'll play next time\n");
        do {
            printf("Choice: ");
            scanf("%d", &mode);
        } while (mode < 0 || mode > 3);

        switch (mode) {
            case 1:
                gameplay_menu(story_list, story_size, set_list, setsize, hero_list, defsize, move_list, move_size, background_list, bg_size);
            case 2:

                system("cls");
                print_header("CONFIGURATION");
                printf("Use your imagination to create wild things!\nPlease choose wisely!\n");
                set_color(96); printf("[1]"); esc_color(); printf(" Character Module\n");
                set_color(96); printf("[2]"); esc_color(); printf(" Storyline Module\n");
                set_color(96); printf("[3]"); esc_color(); printf(" Moves Module\n");
                set_color(96); printf("[4]"); esc_color(); printf(" Go Back to Main Menu\n");

                do {
                    printf("Choice: ");
                    scanf("%d", &c_mode);
                } while (c_mode < 0 || c_mode > 4);

                if (c_mode == 1)
                    character_menu(hero_list, &defsize, set_list, &setsize, move_list, &move_size, story_list, story_size);
                else if (c_mode == 2)
                    story_menu(story_list, story_size, hero_list, defsize, set_list, setsize, move_list, move_size);
                else if (c_mode == 3)
                    moves_menu(move_list, move_size, hero_list, defsize, set_list, setsize);
                else
                    main_menu();

                break;
            case 3:

                printf("Goodbye Player!\n");
                exit(0);
            default: printf("No such choice!\n");
        }
        printf("\nRun main menu again?\n");
        set_color(96); printf("[1]"); esc_color(); printf(" Yes\n");
        set_color(96); printf("[0]"); esc_color(); printf(" No\n");

        do {
            printf("Choice: "); scanf("%d", &running);
        } while (running<0 || running>1);
    }
}
void windows_ready()
{
#ifdef _WIN32
    DWORD dwOldInputMode, dwOldOutputMode;
    HANDLE hStdin, hStdout;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdin, &dwOldInputMode);
    GetConsoleMode(hStdout, &dwOldInputMode);
    SetConsoleMode(hStdin, dwOldInputMode);
    SetConsoleMode(hStdout, dwOldInputMode);
    SetConsoleMode(hStdin, dwOldInputMode);
    SetConsoleMode(hStdout, dwOldOutputMode);
#endif
 }

int main()
{
    windows_ready();
    main_menu();
    return 0;
}
