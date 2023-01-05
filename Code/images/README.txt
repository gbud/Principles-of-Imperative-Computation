15-122 Principles of Imperative Computation
Images

==========================================================

Files you will modify:
   imageutil.c0          - Skeleton image helper functions
   images-test.c0        - Test your manipulations (optional)

Files you won't modify:
   remove-green.c0       - You can modify this to respect the pixel/imageutil
                           interfaces if you'd like
   remove-green-main.c0  - Runs remove_green()
   remove-green-test.c0  - Some unit tests for remove_green()
   stego-main.c0         - Runs hide() and unhide()
   crop-main.c0          - Runs crop()
   resize-main.c0        - Runs stretch_horizontally() and upsize()

Files that don't exist yet:
   pixel.c0              - Copy pixel-int.c0 over from Programming 2 (Pixels)
   stego.c0              - Task 3
   crop.c0               - Task 4
   resize.c0             - Task 5

Data:
   img/g5.png
   img/carnegie.png
   img/scs.png
   img/cmu.png
   img/tinytestpattern.png        - 3x2 image, may be good for testing
   img/g5-remove-green.png        - result of remove_green()
   img/g5-remove-green-bug.png    - result of a buggy remove_green()
   img/bee.png                    - cover image in the writeup
   img/dish.png                   - secret image in the writeup
   img/dish_in_bee.png            - stego image in the writeup
   img/recovered_dish.png         - recovered secret in the writeup
   img/cropped_bee.png            - cropped image in the writeup
   img/resized_bee.png            - dish.png resized with k=3
   img/surprise.png               - mystery stego image

==========================================================

Using the imagediff utility: you have been given the result of running
remove_green on both a correct and a buggy implementation of
g5.png. Running the following command shows that the two images differ
by 600 pixels.

   % imagediff -i img/g5-remove-green.png -j img/g5-remove-green-bug.png
   Loaded image img/g5-remove-green.png. Dimensions are 800 by 600.
   Loaded image img/g5-remove-green-bug.png. Dimensions are 800 by 600.
   Number of pixels with different colors: 600 out of 480000.
   0

If you want to visually see where these 600 different pixels actually
are, you can get imagediff to print out a new image that highlights
the places where differences occured:

   % imagediff -i img/g5-remove-green.png -j img/g5-remove-green-bug.png -o diff.png
   % display diff.png &

In addition to writing test cases, this can be a useful way of
debugging your code.

==========================================================

Compiling remove-green and test cases:
   % cc0 -d -W -w -o remove-green-test pixel.c0 imageutil.c0 remove-green.c0 remove-green-test.c0
   % ./remove-green-test

   % cc0 -d -W -w -o remove-green pixel.c0 imageutil.c0 remove-green.c0 remove-green-main.c0
   % ./remove-green -i img/g5.png -o img/g5-my-output.png
       (This creates the output file img/g5-my-output.png. If you
        left off the "-o img/g5-my-output.png" part, the output
        file would be called img/g5_remove-green.png.)

Compiling your unit tests. You can omit stego.c0, crop.c0 or resize.c0 if you haven't written test cases for the functions in them:
   % cc0 -d -W -w -o images-test pixel.c0 imageutil.c0 stego.c0 crop.c0 resize.c0 images-test.c0
   % ./images-test

Compiling stego:
   % cc0 -d -W -w -o stego pixel.c0 imageutil.c0 stego.c0 stego-main.c0
   % ./stego -c img/bee.png -s img/dish.png -o my_stego.png -r my_recovered.png -q 3
       (This creates the output files my-stego.png and my_recovered.png .
        Call ./stego without arguments for other usages.)
   % imagediff -i my_stego.png -j img/dish_in_bee.png
   % imagediff -i my_recovered.png -j img/recovered_dish.png

Compiling crop:
   % cc0 -d -W -w -o crop pixel.c0 imageutil.c0 crop.c0 crop-main.c0
   % ./crop -i img/bee.png -l 86 -t 47 -w 164 -h 240 -o my_cropped.png
       (This creates the output file my_cropped.png .
        Call ./crop without arguments for other usages.)
   % imagediff -i my_cropped.png -j img/cropped_bee.png

Compiling resize:
   % cc0 -d -W -w -o resize pixel.c0 imageutil.c0 resize.c0 resize-main.c0
   % ./resize -i img/bee.png -k 3 -s my_stretched.png -o my_resized.png
       (This creates the output files my_stretched.png and my_resized.png .
        Call ./resize without arguments for other usages.)
   % imagediff -i my_stretched.png -j img/stretched_bee.png
   % imagediff -i my_resized.png -j img/resized_bee.png

Compiling manipulate:
   % cc0 -d -W -w -o manipulate pixel.c0 imageutil.c0 stego.c0 crop.c0 resize.c0 manipulate.c0 manipulate-main.c0
   % ./manipulate -i img/g5.png -o img/g5-my-manip.png

==========================================================

Submitting from the command line on andrew:
   % autolab122 handin images imageutil.c0 stego.c0 crop.c0 resize.c0 images-test.c0
or
   % autolab122 handin images imageutil.c0 stego.c0 crop.c0 resize.c0 images-test.c0 manipulate.c0
then display autolab's feedback by running:
   % autolab122 feedback

Creating a tarball to submit with autolab.cs.cmu.edu web interface:
   % tar -czvf handin.tgz imageutil.c0 stego.c0 crop.c0 resize.c0 images-test.c0
or
   % tar -czvf handin.tgz imageutil.c0 stego.c0 crop.c0 resize.c0 images-test.c0 manipulate.c0
