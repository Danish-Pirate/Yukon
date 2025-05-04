#include <Basic.h>
#include <CUnit.h>
#include "card_deck.h"

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
        (NULL == CU_add_test(pSuite, "test of shuffle_card_deck", test_shuffle_card_deck))
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
