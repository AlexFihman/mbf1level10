create table res(file_id int, loop_no int, execution_time double, p1 double, p2 double);
.mode csv
.import res.csv res
.output result.csv
select count(*), sum(p1), sum(p1*p1), sum(p2), sum(p2*p2) from res;
.output runtime.csv
select sum(execution_time) as execution_time from res;