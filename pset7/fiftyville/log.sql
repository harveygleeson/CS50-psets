-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description FROM crime_scene_reports WHERE year = 2020 AND month = 7 AND day = 28;
-- 10:15am theft, chamberlin sreet courthouse, 3 interviews each mentions courthouse, done today

-- look for who was interviewed on that day
SELECT name FROM interviews
WHERE year = 2020 AND month = 7 AND day = 28;

-- Jose, Eugene, Barbara, Ruth, Eugene, Raymond were interviewed

-- look at each:
SELECT transcript FROM interviews WHERE name = 'Jose';
-- no mention of courthouse from Jose
SELECT transcript FROM interviews WHERE name = 'Eugene';
-- Eugene mentions courthouse, are there two eugenes?
SELECT transcript FROM interviews WHERE name = 'Barbara';
--no mention from Barbara
SELECT transcript FROM interviews WHERE name = 'Ruth';
-- Ruth mentions courthouse
SELECT transcript FROM interviews WHERE name = 'Raymond';
-- Raymond mentions courthouse

-- Eugene, Ruth, Raymond mention courthouse but are there two eugenes?
SELECT id FROM interviews WHERE name = 'Eugene';
-- two eugenes, id no. 159 and 162

SELECT transcript FROM interviews WHERE name = 'Eugene' AND id = 162;
-- Eugene 162 is the one who mentions the courthouse

-- 10:15am theft, chamberlin sreet courthouse, 3 interviews each mentions courthouse, done today
-- Eugene 162, Ruth, Raymond mention courthouse, scroll back to their interviews and see what was said


-- Eugene 162 recognised thief, saw the thief earlier in the day (when?) withdrawing at ATM on fifer street
-- Raymond, thief called someone as they left for under a minute, mentioned flying out of 50ville,
-- Raymond, says thief asked someone to buy ticket, flight tomorrow (29 July, 2020)
-- Ruth, within 10 mins of theft, thief got into car in carpark, security footage?
-- 10:15am theft, chamberlin street courthouse

--check footage from 10.15 to 10.25 
SELECT activity FROM courthouse_security_logs 
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25;
--not very useful, 8 exits no entrances..

--check phonecalls under 1 min duration on that day 
SELECT caller FROM phone_calls 
WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 1;
-- returns nothing, is raymond telling the truth?

SELECT duration FROM phone_calls 
WHERE year = 2020 AND month = 7 AND day = 28;
--duration is in seconds..
SELECT caller FROM phone_calls 
WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60;
-- 10 phone calls made

-- check atm_transactions
SELECT transaction_type FROM atm_transactions 
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location = 'Fifer Street';
-- 8 withdrawals made, leave this for now

-- compare 10 phone callers with the 8 exiters and look for overlap
SELECT name FROM people
WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2020 AND month = 7 AND day = 28 AND duration <= 60)
AND license_plate IN 
(SELECT license_plate FROM courthouse_security_logs 
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25);

-- 4 suspects:
-- Roger, Russell, Evelyn, Ernest
-- did any of them withdraw from the atm?
-- who withdrew on that day?
SELECT name FROM people 
WHERE id IN 
(SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions 
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw'));

-- who overlaps?
SELECT name FROM people
WHERE name IN ('Roger', 'Russell', 'Evelyn', 'Ernest')
AND name IN (SELECT name FROM people 
WHERE id IN 
(SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions 
WHERE year = 2020 AND month = 7 AND day = 28
AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')));

-- Russell, Ernest are the two suspects
-- Both made a withdrawal on the day
-- Both exited the courthouse within 10 mins of the crime
-- Both made a phone call under one minute on the day 
-- Did either fly? How many flights on the 29th? What is our data?
SELECT city FROM airports;
--flew from Fiftyville

SELECT id FROM airports WHERE city = 'Fiftyville';
-- id = 8

SELECT city FROM airports
WHERE id IN 
(SELECT destination_airport_id FROM flights
WHERE origin_airport_id = 8
AND year = 2020 AND month = 7 AND day = 29);

-- outbound flights went to:
-- Chicago, london, boston, tokyo, SanFran

-- have they flown?
SELECT flight_id FROM passengers
WHERE passport_number IN 
(SELECT passport_number FROM people 
WHERE name IN ('Russell', 'Ernest'));

-- 4 times, any on the right day?
SELECT id FROM flights
WHERE id IN 
(SELECT flight_id FROM passengers
WHERE passport_number IN 
(SELECT passport_number FROM people 
WHERE name IN ('Russell', 'Ernest')))
AND year = 2020 AND month = 7 AND day = 29; 
-- 2 on right day, flight id: 18, 36
-- back to interviews, earliest flight was the one they were on!

SELECT hour FROM flights
WHERE id IN (18, 36);

-- 8 or 16, must be 8 o clock flight
SELECT id FROM flights WHERE id IN (18, 36) AND hour = 8;
-- flight id 36

-- who was on it?
SELECT name FROM people
WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id = 36);
-- Ernest was on the flight, must be the robber

SELECT city FROM airports
WHERE id = 
(SELECT destination_airport_id FROM flights WHERE id = 36);
-- went to london

--Who was the accomplice?
SELECT name FROM people
WHERE phone_number =
(SELECT receiver FROM phone_calls
WHERE caller = 
(SELECT phone_number FROM people 
WHERE name = 'Ernest')
AND year = 2020 AND month = 7 AND day = 28 AND duration <= 60);
-- Berthold!
