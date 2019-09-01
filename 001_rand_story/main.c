#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "catarray.h"

size_t catIndex(catarray_t * words, char * cat) {
  //returns the index of a particular category in a catarray_t *
  for (int i = 0; i < words->n_cats; i++)
    if (strcmp(words->catlist[i]->category, cat) == 0)
      return i;

  return -1;
}

size_t addCat(catarray_t * words, char * cat) {
  //adds a new category to a catarray_t *
  size_t cat_len = ++words->n_cats;
  words->catlist = realloc(words->catlist, sizeof(*(words->catlist)) * cat_len);

  category_t * new_tag = malloc(sizeof(*new_tag));
  new_tag->category = cat;
  new_tag->wordlist = malloc(sizeof(*new_tag->wordlist));
  assert(new_tag->wordlist != NULL);
  new_tag->n_words = 0;

  words->catlist[cat_len - 1] = new_tag;
  return cat_len - 1;
}

void addWordToCat(catarray_t * words, char * word, int idx) {
  //adds a new word to an existing category
  size_t word_len = ++words->catlist[idx]->n_words;
  words->catlist[idx]->wordlist = realloc(
      words->catlist[idx]->wordlist, sizeof(*(words->catlist[idx]->wordlist)) * word_len);
  words->catlist[idx]->wordlist[word_len - 1] = word;
}

void addWord(catarray_t * words, char * line) {
  //adds a words to a category in a catarray_t *
  char * p = strchr(line, '\n');
  if (p != NULL)
    *p = '\0';

  p = strchr(line, ':');
  assert(p != NULL);
  *p = '\0';
  char * new_word = malloc(sizeof(*new_word) * (strlen(p + 1) + 1));
  strcpy(new_word, p + 1);

  size_t index = catIndex(words, line);
  if (index == -1) {
    char * new_line = malloc(sizeof(*new_line) * (strlen(line) + 1));
    strcpy(new_line, line);
    index = addCat(words, new_line);
  }
  addWordToCat(words, new_word, index);
}

void parseLine() {
  //handles one line read from the template file
  ;
}

void handleUsedlist(usedlist_t * usedlist, char * word) {
  usedlist->wordlist =
      realloc(usedlist->wordlist, sizeof(*(usedlist->wordlist)) * ++(usedlist->n_words));
  char * used = malloc(sizeof(char) * (strlen(word) + 1));
  strcpy(used, word);
  usedlist->wordlist[usedlist->n_words - 1] = used;
}

void replaceBlank(char * line, catarray_t * words, usedlist_t * usedlist) {
  //replaces a single blank with the appropriate word
  char * start = strchr(line, '_');
  char * last = NULL;
  if (start == NULL) {
    printf("%s", line);
    return;
  }

  char * cat = malloc(sizeof(*cat) * (start - line + 1));
  strncpy(cat, line, start - line);
  cat[start - line] = '\0';
  printf("%s", cat);

  while ((last = strchr(start + 1, '_')) != NULL) {
    cat = realloc(cat, sizeof(*cat) * (last - start + 1));
    strncpy(cat, start, last - start);
    cat[last - start] = '\0';
    size_t flag = 0;
    char * remain;
    long int previous = strtol(cat + 1, &remain, 10);
    if (previous > 0 && !*remain) {
      assert(previous <= usedlist->n_words);
      printf("%s", usedlist->wordlist[(usedlist->n_words) - previous]);  // here
      handleUsedlist(usedlist, usedlist->wordlist[(usedlist->n_words) - previous]);
      flag = 1;
      start = last + 1;
    }
    else {
      for (int i = 0; i < words->n_cats; i++) {
        if (strcmp(cat + 1, words->catlist[i]->category) == 0) {
          size_t num = rand() % words->catlist[i]->n_words;
          printf("%s", words->catlist[i]->wordlist[num]);
          handleUsedlist(usedlist, words->catlist[i]->wordlist[num]);
          flag = 1;
          start = last + 1;
          break;
        }
      }
    }
    if (!flag) {
      printf("%s", cat);
      start = last;
    }
  }
  printf("%s", start);
  free(cat);
}

catarray_t * readWords(FILE * f) {
  catarray_t * words = malloc(sizeof(*words));
  assert(words != NULL);
  words->n_cats = 0;
  words->catlist = NULL;

  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    addWord(words, line);
    free(line);
    line = NULL;
  }
  free(line);
  return words;
}

void readStory(FILE * f, catarray_t * words) {
  char * line = NULL;
  size_t sz = 0;
  usedlist_t * usedlist = malloc(sizeof(*usedlist));
  usedlist->wordlist = NULL;
  usedlist->n_words = 0;
  //  parseLine();
  while (getline(&line, &sz, f) >= 0) {
    replaceBlank(line, words, usedlist);
    free(line);
    line = NULL;
  }
  free(line);
  for (size_t i = 0; i < usedlist->n_words; i++)
    free(usedlist->wordlist[i]);
  free(usedlist->wordlist);
  free(usedlist);
}

void freeWords(catarray_t * words) {
  for (size_t i = 0; i < words->n_cats; i++) {
    for (size_t j = 0; j < words->catlist[i]->n_words; j++) {
      free(words->catlist[i]->wordlist[j]);
    }
    free(words->catlist[i]->wordlist);
    free(words->catlist[i]->category);
    free(words->catlist[i]);
  }
  free(words->catlist);
  free(words);
}

int main(int argc, char ** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: ./rand_story story.txt words.txt n");
    return EXIT_FAILURE;
  }

  long int seed = strtol(argv[3], NULL, 10);
  if (seed < 1) {
    fprintf(stderr, "n must be an int >0\n");
    return EXIT_FAILURE;
  }
  srandom(seed);

  FILE * words_file = fopen(argv[2], "r");
  if (words_file == NULL) {
    fprintf(stderr, "%s cannot be opened\n", argv[2]);
    return EXIT_FAILURE;
  }

  FILE * story_file = fopen(argv[1], "r");
  if (story_file == NULL) {
    fprintf(stderr, "%s cannot be opened\n", argv[1]);
    return EXIT_FAILURE;
  }

  catarray_t * words = readWords(words_file);
  if (words == NULL) {
    fprintf(stderr, "No words or invalid format\n");
    return EXIT_FAILURE;
  }
  /*
  for (int i = 0; i < words->n_cats; i++) {
    printf("%s\n", words->catlist[i]->category);
    for (int j = 0; j < words->catlist[i]->n_words; j++) {
      printf(" %s", words->catlist[i]->wordlist[j]);
    }
    printf("\n");
  }
*/
  readStory(story_file, words);
  freeWords(words);
  fclose(words_file);
  fclose(story_file);
  return EXIT_SUCCESS;
}
