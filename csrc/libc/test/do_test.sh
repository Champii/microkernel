#!/bin/sh

perform_diff()
{
diff -n expected/$1 $1 > /dev/null

if [ $? -ne 0 ]; then
  echo "test FAIL"
  rm -f serv_pl.log serv_pg.log serv_io.log test.log
  exit 1
fi
}

unlink /dev/mqueue/serv_pl
unlink /dev/mqueue/serv_pg
unlink /dev/mqueue/serv_io

./dummy_serv_pl > serv_pl.log&
PID_PL=$!

sleep 2

./dummy_serv_pg > serv_pg.log&
PID_PG=$!
./dummy_serv_io > serv_io.log&
PID_IO=$!

./test > test.log

kill -9 $PID_PL
kill -9 $PID_PG
kill -9 $PID_IO

unlink /dev/mqueue/serv_pl
unlink /dev/mqueue/serv_pg
unlink /dev/mqueue/serv_io

perform_diff serv_pl.log
perform_diff serv_pg.log
perform_diff serv_io.log
perform_diff test.log

rm -f serv_pl.log serv_pg.log serv_io.log test.log
echo "test PASS"
