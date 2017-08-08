#TrainControlProject

This was one of my first arduino projects. I rebuilt a trainset my father had started
as a _decoration_ around the holiday tree. This one controls a small troly as it moves
around a track, switching signal lights, channging speed as well as stopping at stations.

The part I found nifty was the non-blocking mechanism I devised for controlling the train.
As a consequence commands read from a remote are _queued_ into a control sequence that 
allows the train's actions to be modified without adversely affecting the original 
plan. This is cool. If no one, especially small ones playng with the remote, don't do anything
for a period of time the train returns to it's base routines.
 
