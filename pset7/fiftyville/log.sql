-- Log of duck theft investigation

-- Check details of crime_scene_reports published on date (July 28, 2021) and at location (Humphrey Street)
SELECT description
  FROM crime_scene_reports
 WHERE year = 2021
   AND month = 7
   AND day = 28 AND street = "Humphrey Street";
-- Description indicates:
    -- theft took place at 10:15AM at Humphrey Street bakery
    -- three interviews were conducted on the day of the interview
    -- bakery was mentioned in interview transcript

-- Check interview transcripts that have mentioned of bakery
SELECT name, transcript
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND transcript
  LIKE "%bakery%";
-- There are four interviews by Ruth, Eugene, Raymond and Emma:
    -- 1. Ruth: thief left bakery within 10 minutes of theft, check security cameras
    -- 2. Eugene: thief withdrew some money from ATM on Leggett Street prior to theft
    -- 3. Raymond:
            -- thief called someone and talked to them for < 1 minute after theft
            -- thief said was going to take earliest flight out of Fiftyville the next day (July 29th)
            -- thief asked person to purchase flight
    -- 4. Emma: bakery owner, transcript doesn't seem to be relevant

-- Clue #1 - bakery security cameras for vehicles leaving within 10 minutes of theft
SELECT *
  FROM bakery_security_logs
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute >= 15
   AND minute < 26;
-- 8 vehicles left within 10 minuts of theft:

-- Clue #2 - check Leggett Street ATM records prior to theft (morning of)
SELECT *
  FROM atm_transactions
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = "Leggett Street"
   AND transaction_type = "withdraw";
-- 8 transactions on day of theft (no time to check for records prior to theft)

-- Clue #3 - check phone records for calls after theft for < 1 minute
SELECT *
  FROM phone_calls
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60;
-- 9 phone calls matching description

-- Clue #3 - check earliest flight out of Fiftyville on July 29th, first check flight id of Fiftyville
SELECT id
  FROM airports
 WHERE city = "Fiftyville";
-- Fiftyville airport ID is 8

-- Clue #3 - now check for flights
SELECT *
  FROM flights
 WHERE origin_airport_id = 8
   AND year = 2021
   AND month = 7
   AND day = 29
ORDER BY hour, minute
 LIMIT 1;
-- Flight id is 36 and destination airport id is 4

-- Clue #3 - check destination airport city from id
SELECT city
  FROM airports
 WHERE id = 4;
-- Destination airport is New York City!

-- Clue #3 - check who was on flight id 36
SELECT *
  FROM passengers
 WHERE flight_id = 36;
-- 9 passengers on flight

-- Now see if anyone has a passport number on the flight, a license plate from the bakery, a phone number from the call record and a bank account associated with ATM transaction.
SELECT name
  FROM people
 WHERE passport_number IN
        (SELECT passport_number
           FROM passengers
          WHERE flight_id = 36)
   AND license_plate IN
        (SELECT license_plate
           FROM bakery_security_logs
          WHERE year = 2021
            AND month = 7
            AND day = 28
            AND hour = 10
            AND minute >= 15
            AND minute < 26)
   AND phone_number IN
        (SELECT caller
           FROM phone_calls
          WHERE year = 2021
            AND month = 7
            AND day = 28
            AND duration < 60)
   AND id IN
        (SELECT id
           FROM people
          WHERE id IN
                (SELECT person_id
                   FROM bank_accounts
                  WHERE account_number IN
                        (SELECT account_number
                           FROM atm_transactions
                          WHERE year = 2021
                            AND month = 7
                            AND day = 28
                            AND atm_location = "Leggett Street"
                            AND transaction_type = "withdraw")));
-- Bruce is the thief!

-- Now look for the accomplice. This is the person who Bruce called.
SELECT name
  FROM people
 WHERE phone_number IN
        (SELECT receiver
           FROM phone_calls
          WHERE year = 2021
            AND month = 7
            AND day = 28
            AND duration < 60
            AND caller IN
               (SELECT phone_number
                  FROM people
                 WHERE name = "Bruce"));
-- Robin is the accomplice!