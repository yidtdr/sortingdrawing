# Drawing Process of sorting array (V_1.0)
## Used libraries
I used [SFML](https://github.com/SFML/SFML), [ImGui-SFML](https://github.com/SFML/imgui-sfml) and [Dear ImGui](https://github.com/ocornut/imgui) for all the graphical stuff
## How it looks
I uploaded an [Youtube showcase video](https://www.youtube.com/watch?v=hHNEmQ7tJ0o)
(prev. version)
## Some usage
Since there is no dynamic memory allocation, the only way to change amount of elements is to change the hardcoded numbers in src
```
_sortingdrawing.cpp_ and _Arrays.hpp_

#define NUM 300 -> #define NUM {your amount}
```
# To-do
- [ ] Comlete a to-do list
- [ ] Dynamic memory allocation
- [ ] Changing number of elements at runtime
- [ ] Styling ImGui better
- [ ] Changing color of active arrays
- [ ] Adding different methods of sorting
