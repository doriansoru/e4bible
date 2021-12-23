#include "e4bible.h"

//Full path of the bible
static char BIBLE[255];

//Check if a file exists
//return 0 if the file exists and can be read otherwise return 1
int cFileExists(const char * filename) {
	struct stat buffer;
    if (stat(filename, &buffer ) == 0 ) {
        return E4_OK;
    } else {
        return E4_FAILURE;
    }
}


//Get a random verse
char * getRandomVerse(void) {
	sqlite3 *db;
	sqlite3_stmt * statement;
	assert(cFileExists(BIBLE) == E4_OK);
	int size;
    
	int rc = sqlite3_open(BIBLE, &db);
	check(rc == SQLITE_OK, "Cannot open database: %s", sqlite3_errmsg(db));
		
	rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM verses", -1, &statement, NULL);
		
	check(rc == SQLITE_OK, "Cannot prepare the SQL statement");
		
	int stepCountQuery = sqlite3_step(statement);
	if (stepCountQuery == SQLITE_ROW) {						
		size = sqlite3_column_int(statement, 0);
		sqlite3_finalize(statement);
	}
		
	//commented to get a more "real" random number
	//srand(time(NULL));
	//int randomVerseNumber = rand() % size + 1;
	unsigned int randomVerseNumber;
	ssize_t random_res = -1;
	do {
		random_res = getrandom(&randomVerseNumber, sizeof(unsigned int), GRND_NONBLOCK);
	} while (random_res == -1);
	randomVerseNumber = randomVerseNumber % (size + 1);
			
	rc = sqlite3_prepare_v2(db, "SELECT * FROM verses INNER JOIN books ON books.id=verses.book WHERE verses.rowid= ?" , -1, &statement, NULL);
	check(rc == SQLITE_OK, "Cannot select a verse.");
	sqlite3_bind_int(statement, 1, randomVerseNumber);
			
	sqlite3_step(statement);
	int chapter = sqlite3_column_int(statement, 1);
	int verseNumber = sqlite3_column_int(statement, 2);
	char * verseText = (char *) sqlite3_column_text(statement, 3);
	char * const book = (char *) sqlite3_column_text(statement, 5);
		
	//Formats verseText by putting new line characters
	int lastSpace;
	int i;
	if (strlen(verseText) > MAX_VERSE_LINE_LENGTH) {
		for (i = 0; i < strlen(verseText); i++) {
			//Current character
			char c = verseText[i];
			//Is it a space?
			if (isspace(c)) {
				lastSpace = i;
			}
			if ((i > 0) && ((i % MAX_VERSE_LINE_LENGTH) == 0)) {
				verseText[lastSpace] = '\n';
			}
		}
	}
		
	//Book space chapter:verseNumber \n verseText
	int verseSize = strlen(book) + 1 + (floor(log10(abs(chapter))) + 1) + 1 + (floor(log10(abs(verseNumber))) + 1) + 1 + strlen(verseText) + 1;
	char * verse = malloc(verseSize);
	snprintf(verse, verseSize, "%s %d:%d\n%s", book, chapter, verseNumber, verseText);
		
	sqlite3_finalize(statement);
	sqlite3_close_v2(db);
	return verse;

	error:
		if (statement) {
			sqlite3_finalize(statement);
		}
		if (db) {
			sqlite3_close(db);
		}
		exit(EXIT_FAILURE);
		
}

int main(int argc, char * argv[]){
    char * verse;
      	
    setlocale(LC_ALL, "");
    
    //Config directory home
	snprintf(BIBLE, sizeof(BIBLE), "%s/%s", CONFIG_DIR, "bible.sqlite3");
	assert(strlen(BIBLE) <= MAX_BIBLE_FILE_SIZE);
	
	verse = getRandomVerse();
	printf("%s\n", verse);
	free(verse);
    return EXIT_SUCCESS;
}
