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
 * Minimum number of majors at the faculty.
 */
static const int DEFAULT_K = 5;

/**
 * Minimum number of students on given major.
 */
static const int N_RANGE_MIN = 80;

/**
 * Maximum number of students on given major.
 */
static const int N_RANGE_MAX = 160;

/**
 * Time that student has to prepere the answer.
 */
static const int DEFAULT_T = 5;

#endif

/**
 * The number and names of examination boards.
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
  char board_name; /**< Name, either 'A' or 'B'. @see BOARDS */
};

struct DeanArguments {
  int k; /**< Number of available majors at our faculty. */
};

/**
 * Structure representing all possible arguments, that can be passed to the
 * *main* process.
 *
 * The values k = -1, ns = NULL, ns_len = -1 or t = -1 indicate, that those
 * fields were not parsed correctly, or were not passed in to the program.
 *
 * @see initial_main function for creating unparsed structure.
 * @see parse_main function for parsing command line arguments into this
 * structure.
 * @see validate_main function for validating, if the passed in arguments meet
 * program requirements.
 */
struct MainArguments {
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
 * Creates the BoardArguemnts structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see BoardArguments structure
 * @return BoardArguments structure with values ns = NULL, ns_len = -1 and
 * board_name = '\0'.
 */
struct BoardArguments initial_board();

struct DeanArguments initial_dean();

/**
 * Creates the MainArguments structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see MainArguments structure
 * @return MainArguments structure with values ns = NULL, ns_len = -1, k = -1
 * and t = -1.
 */
struct MainArguments initial_main();

/**
 * Creates the StudentArguments structure filled with values, that
 * signify, that the structure was not parsed yet.
 *
 * @see StudentArguments structure
 * @return StudentArguments structure with all fields set to -1.
 */
struct StudentArguments initial_student();

/**
 * Parses given command line arguments into a structure.
 *
 * @note This function sets errno value.
 * @param argc The standard argument count parameter from main.
 * @param argv The standard argument array from main.
 * @param arguments The pointer to the output structure.
 * @return Wheather the parsing was succesfull or some error occured while
 * parsing arguments (e.g. incorrect flag).
 */
bool parse_board(int argc, char** argv, struct BoardArguments* arguments);
bool parse_dean(int argc, char** argv, struct DeanArguments* arguments);
bool parse_main(int argc, char** argv, struct MainArguments* arguments);
bool parse_student(int argc, char** argv, struct StudentArguments* arguments);

/**
 * Fills the empty (unparsed) fields of the structure to their
 * default values (defined in the requirements).
 *
 * @note This function sets errno value.
 * @param THe pointer to the structure that needs to be filled.
 * @return Wheather the operation was succesfull or some error occured.
 */
bool fill_main(struct MainArguments* arguments);

/**
 * Checks wheather current arguments meet the requirements.
 *
 * @param The structure that needs to be validated.
 * @return Wheather the passed structure meets the requirements.
 */
bool validate_board(struct BoardArguments arguments);
bool validate_dean(struct DeanArguments arguments);
bool validate_main(struct MainArguments arguments);
bool validate_student(struct StudentArguments arguments);

int* _generate_random_ns(int k, int min, int max);

#endif
