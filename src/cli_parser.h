/**
 * @file cli_parser.h
 * @brief Module responsible for parsing and validating command line arguments.
 *
 * @author Filip Cebula
 * @date 22/01/2025
 */

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <stdbool.h>
#include <sys/types.h>

#if defined(ENV_DEV)

// Test amounts
static const int DEFAULT_K = 2;
static const int N_RANGE_MIN = 8;
static const int N_RANGE_MAX = 16;
static const int DEFAULT_T = 2;

#else

/**
 * Constant representing minimum number of majors at the faculty.
 */
static const int DEFAULT_K = 5;

/**
 * Constant defining minimum number of students on given major.
 */
static const int N_RANGE_MIN = 80;

/**
 * Constant defining maximum number of students on given major.
 */
static const int N_RANGE_MAX = 160;

/**
 * Constant representing time, that student has to prepere the answer.
 */
static const int DEFAULT_T = 5;

#endif

/**
 * Constant defining the numver and names of examination boards.
 */
static const char BOARDS[] = {'A', 'B'};
static const ssize_t BOARDS_LENGTH = 2;

/**
 * Structure representing all possible arguments, that can be passed to the
 * *board* process.
 *
 * The values ns = NULL, ns_len = -1 or board_name = '\0' indicate, that those
 * fields were not parsed correctly, or were not passed in to the program.
 *
 * @see initial_board function for creating unparsed structure.
 * @see parse_board function for parsing command line arguments into this
 * structure.
 * @see validate_board function for validating, if the passed in arguments meet
 * program requirements.
 */
struct BoardArguments {
  /**
   * An array that holds the amount of students enrolled in given major *k* as
   * the k-1 element.
   */
  int* ns;
  ssize_t ns_len; /**< Length of the ns array */
  char board_name; /**< Name, either 'A' or 'B'. @see BOARDS */
};

/**
 * Structure representing all possible arguments, that can be passed to the
 * *dean* process.
 *
 * The values k = -1, ns = NULL, ns_len = -1 or t = -1 indicate, that those
 * fields were not parsed correctly, or were not passed in to the program.
 *
 * @see initial_dean function for creating unparsed structure.
 * @see parse_dean function for parsing command line arguments into this
 * structure.
 * @see validate_dean function for validating, if the passed in arguments meet
 * program requirements.
 */
struct DeanArguments {
  /**
   * An array that holds the amount of students enrolled in given major *k* as
   * the k-1 element.
   */
  int* ns;
  ssize_t ns_len;
  int k; /**< Number of available majors at our faculty. */
  int t; /**< Time that student has to prepare the answear. */
};

/**
 * Structure representing all possible arguments, that can be passed to the
 * *student* process.
 *
 * The values k = -1, n = -1 or t = -1 indicate, that those fields were not
 * parsed correctly, or were not passed in to the program.
 *
 * @see initial_student function for creating unparsed structure.
 * @see parse_student function for parsing command line arguments into this
 * structure.
 * @see validate_student function for validating, if the passed in arguments 
 * meet program requirements.
 */
struct StudentArguments {
  int k; /**< Student's major */
  int n; /**< Student's number on the list */
  int t; /**< Time that student has to prepare the answear. */
};

/**
 * Function that creates the BoardArguemnts structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see BoardArguments structure
 * @return BoardArguments structure with values ns = NULL, ns_len = -1 and
 * board_name = '\0'.
 */
struct BoardArguments initial_board();

/**
 * Function that creates the DeanArguments structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see DeanArguments structure
 * @return DeanArguments structure with values ns = NULL, ns_len = -1, k = -1
 * and t = -1.
 */
struct DeanArguments initial_dean();

/**
 * Function that creates the StudentArguments structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see StudentArguments structure
 * @return StudentArguments structure with all fields set to -1.
 */
struct StudentArguments initial_student();

/**
 * Function that parses given command line arguments into a structure.
 *
 * @note This function sets errno value.
 * @param argc The standard argument count parameter from main.
 * @param argv The standard argument array from main.
 * @param args The pointer to the output structure.
 * @return Wheather the parsing was succesfull or some error occured while
 * parsing arguments (e.g. incorrect flag).
 *
 * @see parse_dean for the program specific parsing.
 * @see parse_student for the program specific parsing.
 */
bool parse_board(int argc, char** argv, struct BoardArguments* args);
bool parse_dean(int argc, char** argv, struct DeanArguments* args);
bool parse_student(int argc, char** argv, struct StudentArguments* args);

/**
 * Functions that fills the empty (unparsed) fields of the structure to their
 * default values (defined in the requirements).
 *
 * @note This function sets errno value.
 * @param THe pointer to the structure that needs to be filled.
 * @return Wheather the operation was succesfull or some error occured.
 */
bool fill_dean(struct DeanArguments* args);

/**
 * Function that checks wheather current arguments meet the requirements.
 *
 * @param The structure that needs to be validated.
 * @return Wheather the passed structure meets the requirements.
 *
 * @see validate_dean
 * @see validate_student
 */
bool validate_board(struct BoardArguments args);
bool validate_dean(struct DeanArguments args);
bool validate_student(struct StudentArguments args);

int* _generate_random_ns(int k, int min, int max);

#endif
