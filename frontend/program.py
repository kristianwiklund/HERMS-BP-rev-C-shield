#
# simple machine for running a mash program
#
# the machine consumes a text file with commands
#
# tm <temp> set mash setpoint to temperature <temp>. Will cause error if the HLT is colder than <temp>
# th <temp> set HLT setpoint to temperature <temp>
# wait_time <minutes> wait for <minutes> minutes, then continue
# stepname <text> displays the <text> as step name in the gui
# wait_tm <temp> wait until mash temp is <temp>
# wait_th <temp> wait until hlt temp is <temp>
# beep sound the alarm
# requester <text> show a requester with text and wait for the user to click OK

