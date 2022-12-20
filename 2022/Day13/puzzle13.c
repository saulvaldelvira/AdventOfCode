#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>

#ifdef DEGUB
    #define FILENAME "test.txt"
#else
    #define FILENAME "input.txt"
#endif

typedef struct Packet {
    struct PacketValue  *array;
    size_t               size;
} Packet;

typedef union ValueWrapper{
    int64_t number;
    struct Packet *list;
} ValueWrapper;

typedef struct PacketValue{
    union ValueWrapper value;
    bool is_list;    
} PacketValue;

static size_t packet_lenght(char *line){
    //return 0 on empty line
    if(line[0] == '\n' || line[0] == '\0')
        return 0;
    assert(line[0] == '[');

    int64_t depth = -1;         // Current depth on the nested list. (0 = top level list, -1 = outside the list)
    size_t  index = 0;          // Index on the string
    size_t  count = 0;          // Count of elements at the top level of the list
    char    cur_char = '\0';    //Current character

    // Read until the end of the top level list
    do{
        cur_char = line[index++];

        switch (cur_char){
        case '[':
            if (depth++ == 0) //Increases depth and if at the top level, count the element.
                count++;
            break;
        case ']':
            depth--;
            break;
        case ',':
            //Ignore
            break;
        default:
            if(isdigit(cur_char)){
                if(depth == 0)
                    count++;
                while(isdigit(line[index+1])){ //move until the end of the number
                    cur_char = line[index++];
                }
            }else{
                fprintf(stderr, "Error 65: Malformated Input (%s)\n", line);
                abort();
            }
            break;
        }
    }while(depth > -1 && cur_char != '\0');

    if(depth == -1)
        return count;

    // The string ended before the list was closed
    fprintf(stderr, "Error: Unbalanced brackets\n");
    abort();
}

//parses a packet from a line
static Packet* packet_parse(char *line){
    // Number of elements on top level of the list
    size_t length = packet_lenght(line);

    // Alocate memory for the top level list
    Packet *packet = calloc(1, sizeof(Packet));
    PacketValue *value_array = length > 0 ? (PacketValue*)calloc(length, sizeof(PacketValue)) : NULL;

    if(!packet || (!value_array && length > 0)){
        fprintf(stderr, "Error: No enough memory\n");
        abort();
    }

    // Initialize the list
    *packet = (Packet){
        .array = value_array,
        .size = length
    };

    if(length == 0)
        return packet;

    int64_t depth = -1;     // Current depth on the nested list
    size_t char_index = 0;  // Index of the current character
    size_t list_index = 0;  // Index of the current value on the list

    char cur_char;

    do{
        cur_char = line[char_index++];
        switch (cur_char){
        case '[':
            depth++;    // Move down the depth

            // If at the top level, a nested list has been found
            if(depth == 1){
                // Parse the packet list and add it
                Packet *nested_packet = packet_parse(&line[char_index-1]);
                value_array[list_index++] = (PacketValue){
                    .value.list = nested_packet,
                    .is_list = true
                };
            }
            break;
        case ']':
            depth--;
            if(depth == -1)
                return packet;
            break;
        case ',':
            //Ignore
            break;
        default:
            // A digit should be found here
            if(isdigit(cur_char)){
                if(depth == 0){
                    int64_t number = atol(&line[char_index-1]);
                    value_array[list_index++] = (PacketValue){
                        .value.number = number,
                        .is_list = false
                    };
                }
                while(isdigit(line[char_index])){
                    cur_char = line[char_index++];
                }
            }
            break;
        }
    }while (cur_char != '\0' && cur_char != '\n');

    // On a valid input file, this error should never be reached
    fprintf(stderr, "Error 152: Malformatted input\n");
    abort();
}

// Compare two packets and return:
// +1: if packet_1 >  packet_2
//  0: if packet_1 == packet_2
// -1: if packet_1 <  packet_2
static int packet_compare(Packet *packet_1, Packet *packet_2){
    int result = 0;

    // Get the minimun length between the two packets
    size_t min_size = packet_1->size < packet_2->size ? packet_1->size : packet_2->size;

    // Allocate memory on the stack for the temporary list
    // (used to convert an integer into a list, when the compared types are different)
    static Packet temp_packet;
    static PacketValue temp_value;

    for (size_t i=0; i < min_size; i++){
        // Get a pair of values from the packets
        PacketValue *value_1 = &packet_1->array[i];
        PacketValue *value_2 = &packet_2->array[i];

        // Check how many elements are lists
        int comparison = (int)(value_1->is_list) + (int)(value_2->is_list);

        switch (comparison){
            case 0: // Both integers
                const int64_t int_1 = value_1->value.number;
                const int64_t int_2 = value_2->value.number;
                if(int_1 == int_2)
                    continue;
                return int_1 < int_2 ? -1 : 1;
                break;
            case 1: // One integer and one list
                PacketValue *list_value;
                PacketValue *int_value;

                if(value_1->is_list){
                    list_value = value_1;
                    int_value = value_2;
                }else{
                    list_value = value_2;
                    int_value = value_1;
                }

                // Wrap the integer into a list
                temp_value = (PacketValue){
                    .value.number = int_value->value.number,
                    .is_list = false
                };

                temp_packet = (Packet){
                    .array = &temp_value,
                    .size = 1
                };

                if (list_value == value_1)
                    result = packet_compare(list_value->value.list, &temp_packet);
                else
                    result = packet_compare(&temp_packet, list_value->value.list);

                if(result != 0)
                    return result;
                break;
            case 2: // Both lists
                Packet *list_1 = value_1->value.list;
                Packet *list_2 = value_2->value.list;
                result = packet_compare(list_1, list_2);
                if(result != 0)
                    return result;
                break;
        }
    }

    // If we ran out of elements on a list before a inequality was found,
    // compare the lists sizes
    if (packet_1->size == packet_2->size)
        return 0;
    else if(packet_1->size < packet_2->size)
        return -1;
    else
        return 1;
}

// Helper function for the sorting of an array of packet pointers
static int packet_sorter(const void *packet_1, const void *packet_2){
    Packet *p1 = *(Packet**)packet_1;
    Packet *p2 = *(Packet**)packet_2;
    return packet_compare(p1, p2);
}

void packet_destroy(Packet *packet){
    for (size_t i = 0; i < packet->size; i++){
        if (packet->array[i].is_list){
            packet_destroy(packet->array[i].value.list);
        }
    }
    free(packet->array);
    free(packet);
}

int main(int argc, char **argv){
    FILE *file = fopen(FILENAME, "rt");
    char line[512];
    size_t packet_count = 0;

    while (fgets(line, sizeof(line), file)){
        if(line[0] == '[')
            packet_count++;
    }

    rewind(file);

    // Allocate memory for an array of packet pointers
    // (size is the packet count plus 2 in order to account for the divider packets)
    Packet **packet_array = (Packet**)calloc(packet_count + 2, sizeof(Packet));
    if(!packet_array){
        fprintf(stderr, "Error: No enough memory\n");
        abort();
    }

    size_t packet_index = 0;
    while (fgets(line, sizeof(line), file)){
        if(line[0] == '\n') continue;
        packet_array[packet_index++] = (Packet*)packet_parse(line);
    }

    fclose(file);

    // Count the pairs of packets in the wrong order
    size_t solution_1 = 0;
    for (size_t i = 0; i < packet_count; i+=2){
        int result = packet_compare(packet_array[i], packet_array[i+1]);
        if (result == -1) solution_1 += (i / 2) + 1;
    }
    
    printf("Part 1: %lu\n", solution_1);

    // Append the two divider packets to the end of the array
    char divider_text_1[] = "[[2]]\n";
    char divider_text_2[] = "[[6]]\n";

    Packet *divider_packet_1 = packet_parse(divider_text_1);
    Packet *divider_packet_2 = packet_parse(divider_text_2);

    packet_array[packet_index++] = divider_packet_1;
    packet_array[packet_index++] = divider_packet_2;

    // Sort in ascendent order
    qsort(packet_array, packet_count+2, sizeof(Packet*), &packet_sorter);

    size_t solution_2 = 1;
    for (size_t i = 0; i < packet_count; i++){
        Packet *packet = packet_array[i];
        if(packet == divider_packet_1 || packet == divider_packet_2){
            solution_2 *= (i+1);
        }
    }

    printf("Part 2: %lu\n", solution_2);

    for (size_t i = 0; i < packet_index; i++)
        packet_destroy(packet_array[i]);
    
    free(packet_array);

    return 0;
}