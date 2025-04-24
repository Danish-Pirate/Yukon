#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../include/card_deck.h"
#include "command_parser.h"

void test_read_input() {
    FILE *original_stdin = stdin;



    // Test Case 1: LD command with file path
    FILE *temp_file1 = tmpfile();
    fputs("LD test_deck.txt", temp_file1);
    rewind(temp_file1);
    stdin = temp_file1;

    Command cmd1 = getCommandInput();
    CU_ASSERT_EQUAL(cmd1.type, CMD_LD);
    CU_ASSERT_STRING_EQUAL(cmd1.ld.filePath, "test_deck.txt");
    fclose(temp_file1);

    // Test Case 2: LD command without file path
    FILE *temp_file2 = tmpfile();
    fputs("LD", temp_file2);
    rewind(temp_file2);
    stdin = temp_file2;

    Command cmd2 = getCommandInput();
    CU_ASSERT_EQUAL(cmd2.type, CMD_LD);
    CU_ASSERT_STRING_EQUAL(cmd2.ld.filePath, "\0");
    fclose(temp_file2);

    // Test Case 3: SW command
    FILE *temp_file3 = tmpfile();
    fputs("SW", temp_file3);
    rewind(temp_file3);
    stdin = temp_file3;

    Command cmd3 = getCommandInput();
    CU_ASSERT_EQUAL(cmd3.type, CMD_SW);
    fclose(temp_file3);

    // Test Case 4: SI command with split index
    FILE *temp_file4 = tmpfile();
    fputs("SI 5", temp_file4);
    rewind(temp_file4);
    stdin = temp_file4;

    Command cmd4 = getCommandInput();
    CU_ASSERT_EQUAL(cmd4.type, CMD_SI);
    CU_ASSERT_EQUAL(cmd4.si.splitIndex, 5);
    fclose(temp_file4);

    // Test Case 5: SI command without split index
    FILE *temp_file5 = tmpfile();
    fputs("SI", temp_file5);
    rewind(temp_file5);
    stdin = temp_file5;

    Command cmd5 = getCommandInput();
    CU_ASSERT_EQUAL(cmd5.type, CMD_SI);
    CU_ASSERT_EQUAL(cmd5.si.splitIndex, -1);
    fclose(temp_file5);

    // Test Case 6: SR command
    FILE *temp_file6 = tmpfile();
    fputs("SR", temp_file6);
    rewind(temp_file6);
    stdin = temp_file6;

    Command cmd6 = getCommandInput();
    CU_ASSERT_EQUAL(cmd6.type, CMD_SR);
    fclose(temp_file6);

    // Test Case 7: SD command with file path
    FILE *temp_file7 = tmpfile();
    fputs("SD save_deck.txt", temp_file7);
    rewind(temp_file7);
    stdin = temp_file7;

    Command cmd7 = getCommandInput();
    CU_ASSERT_EQUAL(cmd7.type, CMD_SD);
    CU_ASSERT_STRING_EQUAL(cmd7.ld.filePath, "save_deck.txt");
    fclose(temp_file7);

    // Test Case 8: SD command without file path
    FILE *temp_file8 = tmpfile();
    fputs("SD", temp_file8);
    rewind(temp_file8);
    stdin = temp_file8;

    Command cmd8 = getCommandInput();
    CU_ASSERT_EQUAL(cmd8.type, CMD_SD);
    CU_ASSERT_STRING_EQUAL(cmd8.ld.filePath, "\0");
    fclose(temp_file8);

    // Test Case 9: QQ command
    FILE *temp_file9 = tmpfile();
    fputs("QQ", temp_file9);
    rewind(temp_file9);
    stdin = temp_file9;

    Command cmd9 = getCommandInput();
    CU_ASSERT_EQUAL(cmd9.type, CMD_QQ);
    fclose(temp_file9);

    // Test Case 10: P command
    FILE *temp_file10 = tmpfile();
    fputs("P", temp_file10);
    rewind(temp_file10);
    stdin = temp_file10;

    Command cmd10 = getCommandInput();
    CU_ASSERT_EQUAL(cmd10.type, CMD_P);
    fclose(temp_file10);

    // Test Case 11: Q command
    FILE *temp_file11 = tmpfile();
    fputs("Q", temp_file11);
    rewind(temp_file11);
    stdin = temp_file11;

    Command cmd11 = getCommandInput();
    CU_ASSERT_EQUAL(cmd11.type, CMD_Q);
    fclose(temp_file11);

    // Test Case 12: Move command - column to column
    FILE *temp_file12 = tmpfile();
    fputs("C1->C2", temp_file12);
    rewind(temp_file12);
    stdin = temp_file12;

    Command cmd12 = getCommandInput();
    CU_ASSERT_EQUAL(cmd12.type, CMD_MV);
    CU_ASSERT_EQUAL(cmd12.mv.fromColumnIndex, 1);
    CU_ASSERT_EQUAL(cmd12.mv.toColumnIndex, 2);
    CU_ASSERT_EQUAL(cmd12.mv.rank, -1);
    CU_ASSERT_EQUAL(cmd12.mv.suit, -1);
    fclose(temp_file12);

    // Test Case 13: Move command - column with specific card to column
    FILE *temp_file13 = tmpfile();
    fputs("C6:4H->C3", temp_file13);
    rewind(temp_file13);
    stdin = temp_file13;

    Command cmd13 = getCommandInput();
    CU_ASSERT_EQUAL(cmd13.type, CMD_MV);
    CU_ASSERT_EQUAL(cmd13.mv.fromColumnIndex, 6);
    CU_ASSERT_EQUAL(cmd13.mv.toColumnIndex, 3);
    CU_ASSERT_EQUAL(cmd13.mv.rank, FOUR);
    CU_ASSERT_EQUAL(cmd13.mv.suit, HEARTS);
    fclose(temp_file13);

    // Test Case 14: Move command - foundation to column
    FILE *temp_file14 = tmpfile();
    fputs("F3->C4", temp_file14);
    rewind(temp_file14);
    stdin = temp_file14;

    Command cmd14 = getCommandInput();
    CU_ASSERT_EQUAL(cmd14.type, CMD_MV);
    fclose(temp_file14);

    // Test Case 15: Move command - column to foundation
    FILE *temp_file15 = tmpfile();
    fputs("C5->F1", temp_file15);
    rewind(temp_file15);
    stdin = temp_file15;

    Command cmd15 = getCommandInput();
    CU_ASSERT_EQUAL(cmd15.type, CMD_MV);
    CU_ASSERT_EQUAL(cmd15.mv.fromColumnIndex, 5);
    CU_ASSERT_EQUAL(cmd15.mv.toColumnIndex, 8); // F1 = 7+1 = 8
    fclose(temp_file15);

    // Test Case 16: Unknown command
    FILE *temp_file16 = tmpfile();
    fputs("INVALID", temp_file16);
    rewind(temp_file16);
    stdin = temp_file16;

    Command cmd16 = getCommandInput();
    CU_ASSERT_EQUAL(cmd16.type, CMD_UNKNOWN);
    fclose(temp_file16);

    // Test Case 17: Empty command
    FILE *temp_file17 = tmpfile();
    fputs("", temp_file17);
    rewind(temp_file17);
    stdin = temp_file17;

    Command cmd17 = getCommandInput();
    CU_ASSERT_EQUAL(cmd17.type, CMD_UNKNOWN);
    fclose(temp_file17);


    // Restore the original stdin
    stdin = original_stdin;
}

void test_load_card_deck() {
    CU_ASSERT_EQUAL(0, 1);
}

void test_save_card_deck() {
    CU_ASSERT_EQUAL(0, 1);
}

void test_move_cards() {
    CU_ASSERT_EQUAL(0, 1);
}

void test_shuffle_card_deck() {
    CU_ASSERT_EQUAL(0, 1);
}

int init_suite() {
    return 0;
}

int clean_suite() {
    return 0;
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("Yukon_Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "test of load_card_deck", test_load_card_deck)) ||
        (NULL == CU_add_test(pSuite, "test of save_card_deck", test_save_card_deck)) ||
        (NULL == CU_add_test(pSuite, "test of move_cards", test_move_cards)) ||
        (NULL == CU_add_test(pSuite, "test of shuffle_card_deck", test_shuffle_card_deck)) ||
        (NULL == CU_add_test(pSuite, "test of read_input", test_read_input))
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    unsigned int failures = CU_get_number_of_failures();

    CU_cleanup_registry();
    return failures;
}
