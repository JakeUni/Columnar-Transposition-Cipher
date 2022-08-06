#include <stdio.h>
#include <stdlib.h>

/**
 *
 * This function takes in a character and returns it capatalized if originially not
 * @param toup - char to up
 * @return - uppercase char
 */
char up(char toup){
    int c = (int)toup;
    if  ( (c < 123 && c > 96)){
        return (char)c-32;
    }
    return (char)c;
}
/**
 * this function will recreate the words as they are to be added
 * this will return the name sorted as it needs to be
 * @param name - the original
 * @param size - the size of the word
 * @param keys - the key[] array which holds the indexes the words will be sorted by
 * @return the sorted word
 */
char *add(char *name, int size, int * keys){

    char *added = malloc((size + 1) * sizeof(char));
    //sorting the word
    for (int i = 0; i < size; i++){
        added[keys[i]] = up(name[i]);
    }
    added[size] = '\0';
    return added;
}

/**
 * this will return if it is a letter or not
 * @param l - this is the char to be checked
 * @return - if it is a letter or not
 */
int isLetter(char l){
    int c = (int)l;
    if  ((c < 91 && c > 64) || (c < 123 && c > 96)){
        return 1;
    }
    return 0;
}

/**
 * this will return the length of the word
 */
int length(char *word){
    int i = 0;
    char *c = word;
    while(*c != '\0'){
        c++;
        i++;
    }
    return i;
}
/**
 * this will return the length of the array of strings
 */
int totalLength(char **words){
    int i = 0;
    char **c;
    c = words;
    while(*c != 0){
        c++;
        i++;
    }
    return i;
}

/**
 * this will increment the size of the array by the amount specified in newSize
 */
char **increaseSize(char ** original, int newSize, int oldSize){
    char **new = malloc((newSize+1) * sizeof(char *));
    for (int i = 0; i < newSize && i < oldSize; i++){
        new[i] = original[i];
    }
    free(original);
    new[newSize] = 0;
    return new;
}
//this function is for reading and sorting the file based on the keys integer array
//when it is decided that a name has been recieved than it will be passed to add with the keys array
//it will return the sorted array
char **fileToArr(char* f, int initial,char *key, int *keys){
    FILE *file  = fopen(f, "r");
    if (file == NULL) {
        printf("no file found");
        exit(1);
    }
    int size = length(key);
    int increase = initial / 4;
    char **word = (char**)malloc((initial)*sizeof(char*)) ;
    char *currWord = malloc(size * sizeof(char));
    int wordCounter = 0;
    int letterCounter = 0;
    int nextC = 0;
    char c ;

    while (nextC != EOF) {
        c = (char) nextC;
        if ((isLetter(c) == 1) && letterCounter < size) {
            currWord[letterCounter] = c;
            letterCounter++;
        }else{
            if (letterCounter == size) {
                if (wordCounter >= initial - 5) {
                    word = increaseSize(word, initial + increase, initial);
                    initial = initial + increase;
                }
                word[wordCounter] = add(currWord, letterCounter, keys);
                wordCounter++;
                letterCounter = 0;
            }
            if ((isLetter(c) == 1) && letterCounter < size) {
                currWord[letterCounter] = c;
                letterCounter++;
            }
        }

        nextC = getc(file);

    }

    if (letterCounter > 0){
            while(letterCounter!=size) {
                currWord[letterCounter] = 'X';
                letterCounter++;
            }
        word[wordCounter++] = add(currWord, letterCounter, keys);
    }

    fclose((FILE *) file);
    free(currWord);
    return(increaseSize(word, wordCounter, initial));
}

//this is the main , key can be changed manually
int main() {
    //create the key
    char key[] = "LOVELACE";
    int size =length(key);
    int *keys =  malloc(size * sizeof(int));

    for(int k=0; k<size;k++){
        keys[k] = k;
    }
    char temp;
    char temp2;
    int tempo;
    int tempo2;
    //sort the key and the key array
    for(int j=0; j<size;j++){
        for(int k=0; k<size;k++){
            if(key[j] < key[k]){
                tempo = keys[k];
                tempo2 = keys[j];

                temp = key[k];
                temp2 = key[j];

                key[k] = temp2;
                key[j] = temp;
                keys[k] = tempo2;
                keys[j] = tempo;
            }
        }
    }
    //sort and get the words
    char** words = fileToArr("text.txt",1000,key, keys);


    FILE *file  = fopen("hashed7.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    for(int i = 0; i < totalLength(words); i++){
        fprintf(file, "%s", words[i]);
    }
    fclose(file);

    return 0;
}