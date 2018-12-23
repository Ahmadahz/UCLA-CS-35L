Bradley Mont
CS 35L Lab 8, Rahul Dhavalikar
Assignment 6. Multithreaded performance

After-Action Report

First off, I did run into a couple issues during this homework. The biggest challenge for me was receiving this 
large program that I had never seen before and then trying to understand the logic of all the code. That probably
took me the most time in the entire homework. After understanding what the code does, I then struggled to figure 
out how to modify the code in order to make it parallelizable. Finally, I realized that all of the individual
pixel operations done in the main function could be executed using multithreading to achieve a faster performance.
Therefore, what I did was I took all of the individual pixel processing code from the main, and I put it into its
own helper function. I then passed that helper function as the third parameter to creating all my threads, which 
means that each thread executed that function once they were created. 

Additionally, after moving that code to its own helper function, I then got multiple errors because I had multiple
variables that I declared in my main function that I also accessed in my helper function. Therefore, I had to 
change some of the variables that I initialized in my main function to global variables. That way, their values
could be accessed from my main function and from the helper function as well.

Finally, it took me awhile to realize that I had to move all of the print statements outside of the helper
function to make sure that everything was printed in the correct order. I couldn't guarantee that the threads
would print in the right order, so I printed once all the threads were joined. Plus, I wanted to print at the
end to avoid any possible race conditions.

Conclusion:
After incorporating multithreading and testing, I got the following output:
time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m42.585s
user	0m42.573s
sys	0m0.003s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m21.576s
user	0m42.942s
sys	0m0.012s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m10.998s
user	0m43.762s
sys	0m0.005s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m5.740s
user	0m44.554s
sys	0m0.005s
mv 8-test.ppm.tmp 8-test.ppm

As shown by the above data, my implementation of SRT improves its performance.
As we double the threads from 1 to 2 to 4 to 8, the real time goes from
42.6s to 21.6s to 11.0s to 5.7s. We see that as we double the amount of threads,
the time is pretty much cut in half each time. That means that our 
multithreading implementation is splitting up the code into parallelizable tasks
that can be executed concurrently. As we can see, this approach using
multithreading significantly increases the speed of our code. 
