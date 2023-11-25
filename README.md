# Sorting drawing v2.0
## Description
Basically self-explanatory, but in case you didn't catch, Sorting Drawing program is made to visually represent how different algorithms actually sort an array
This program is made for 2 (3) general purposes:
- Take a look deeper how it actually looks to sort an array with specific algorithm
- Have some sort of insight of time to sort with different algorithms (take a look at Time problem)
- (It just looks cool, why not)
## What's new
### New algorithms
> Now you can choose one of existing (3, but I'm going to add other algorithms to list too) sorting algorithm at the moment.
### Differ array size
> Implemented slider to change current array size. Currently tested for only 4-300 elements, but you can try going higher by changing it in code: `sortingdrawing.cpp -> ImGui::SliderInt("Array size", &newSize, 4, <insert_your_number>)`
### Showing iterable objects
> Now the elements that are being used right now are highlighted with different colors
### Shuffle iterations
> Fun little feature
## To-do
- Fix bug where it doesn't work right on monitors other that 1920x1080 (but for now you can just go to `Arrays.hpp` and change res in `#define` by yourself)
- Add sound like in those videos
- Add more sorting algorithms
- Fix some weird problem with ImGui that crashes the program when trying to `PRT SCR`

## Time problem
Basically, at this point time counts somewhat like this:
- Start a timer
- Start sorting array
  - Every time you go `this_thread::sleep_for`, make a timer, then add this time to `globals::sleeped`
- After sorting ended, stop the timer and descrease the time by time that the thread was sleeping for
- Done

### This approach is very good when you think of it, but...
...it doesn't actually count the time for initializing a new timer and even if you manually make it so it get's included, there is some weird stuff going on with time when you try to change sleep time. For some reason, with this approach, it still being affected by the sleep time. If you start an algorithm at the speed of 50%, the time appeared is `actual time * ~5.5`. I will try to solve this problem, but for now, if you need not just the representation, but the actual time you can divide the time, the program outputs at 50% sorting speed, by 5.5 and get somewhat real numbers.
### But that's not all!
Let's say you have a game, which has some sort of classes you wanna sort. At this case, the direct solution would be just sorting the classes by some of its attribute. This is how my program is actually working, but there is a way to make it like 7-8 times faster by losing some memory. Instead of solving array of classes and getting classes attributes by functions (you didn't make attributes public, i hope?) before actually getting to sorting, make an array, each element of which corresponds to it's indexed array of classes elements attribute, whereupon you sort for this array and the final step is to go to each element in class array and set the attributes sorted way. This works becase refering to class function is a lot slower then refeting to arrays element

## Some media
### [YouTube](https://www.youtube.com/@y1dtdr)
![image](https://github.com/yidtdr/sortingdrawing/assets/144206744/e5c6edf6-0d3e-4483-8cd7-69368542ba49)
![image](https://github.com/yidtdr/sortingdrawing/assets/144206744/414b7857-e6ba-4713-8141-c47724c637ee)

## Contact me (pls)
[TG](https://t.me/yidtdr)

## Used libraries
I used [SFML](https://github.com/SFML/SFML), [ImGui-SFML](https://github.com/SFML/imgui-sfml) and [Dear ImGui](https://github.com/ocornut/imgui) for all the graphical stuff
