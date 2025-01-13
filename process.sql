create table res(file_id int, loop_no int, execution_time double, l1 int, l2 int, p double);
.mode csv
.import res.csv res
.output result.csv
select l1, l2, sum(p), sum(p*p) from res group by l1, l2;
.output runtime.csv
select sum(e) from (select file_id, loop_no, max(execution_time) e from res group by file_id, loop_no);