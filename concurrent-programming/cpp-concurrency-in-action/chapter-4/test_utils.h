#pragma once

#define EXPECT_THROW_FUTURE_ERROR(statement, error_code)                       \
  EXPECT_THROW(                                                                \
      {                                                                        \
        try {                                                                  \
          statement;                                                           \
        } catch (const std::future_error &e) {                                 \
          EXPECT_EQ(e.code(), error_code);                                     \
          throw;                                                               \
        }                                                                      \
      },                                                                       \
      std::future_error)