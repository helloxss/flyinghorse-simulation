#!/usr/bin/env python
import csv
import itertools
import sys
import re
import math
csv_gpsr_fileobj = open('gpsr_s_nav_tlm.csv', 'rbU')
csv_dm_fileobj = open('refine_log_nspo.csv', 'rbU')
answer_file_name = ""
if len(sys.argv) > 1:
    answer_file_name = sys.argv[1] + "_"
combine_file = open(answer_file_name + 'combine_result_dm_and_gpsr_log.csv', 'w')
csvCursor = csv.writer(combine_file)

dm_arr = csv.reader(csv_dm_fileobj)
gpsr_arr = csv.reader(csv_gpsr_fileobj)
csvHeader = ['sim_time (sec)', 'DM_gps_sow (sec)',
             'DM_SBEE_X ', 'DM_SBEE_Y ', 'DM_SBEE_Z ',
             'DM_VBEE_X ', 'DM_VBEE_Y ', 'DM_VBEE_Z ',
             'GPSR_gps_sow (sec)',
             'GPSR_SBEE_X ', 'GPSR_SBEE_Y ', 'GPSR_SBEE_Z ',
             'GPSR_VBEE_X ', 'GPSR_VBEE_Y ', 'GPSR_VBEE_Z ',
             'GPSR_Length', 'GPSR_Speed ']
csvCursor.writerow(csvHeader)

sim_data_list = []
iter_idx = 0
for dm_row, gpsr_row in itertools.izip(dm_arr, gpsr_arr):
    iter_idx += 1
    sim_data_list = []
    if iter_idx <= 1:
        continue
    print dm_row[0]

    for x in xrange(0,8):
        sim_data_list.append(dm_row[x])
    for x in xrange(0,7):
        sim_data_list.append(gpsr_row[x])
# Calculate GPSR Length
    sim_data_list.append(math.sqrt(float(gpsr_row[1])**2 + float(gpsr_row[2])**2  + float(gpsr_row[3])**2))
# Calculate GPSR Speed
    sim_data_list.append(math.sqrt(float(gpsr_row[4])**2 + float(gpsr_row[5])**2  + float(gpsr_row[6])**2))
    csvCursor.writerow(sim_data_list)

csv_dm_fileobj.close()
csv_gpsr_fileobj.close()
combine_file.close()


print answer_file_name + "combine_result_dm_and_gpsr_log.csv\n"
print "Next step ./error_with_gpsr_answer.py"