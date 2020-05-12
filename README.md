# Predator & Prey Cellular Automaton

Simulating the life of *predators* and *prey*.

## About

The goal of this automaton is to simulate a world inhabited by two types of creatures predators (displayed with red) and prey (displayed with green).

<p align="center">
  <img src="./screeshots/ss.gif">
</p>

At each moment, every creature moves in a random direction.

Prey is rewarded for staying alive. Thus, at each moment its health rises by a certain amount. If it reached a certain threshold, the prey reproduces.

Predators are rewarded for hunting. They lose health at each moment, unless they encounter prey. In that case, the predator heals by consuming the prey and a new predator is born.

After some time, illness starts spreading. Prey becomes orange and predators become purple. Eventually, all creatures perish into nothing and the simulation ends. 

The user can at any moment start a new simulation by pressing **N** or quit by pressing **Q**.

## Getting it to run

## Prerequisites

* [SFML](https://www.sfml-dev.org/) - graphics and event handling

### Install on Debian / Ubuntu

`sudo apt-get install libsfml-dev`

### Install on Arch

`sudo pacman -S sfml`

## Downloading

* [Download v2.0.0](https://github.com/Stefan-Radu/Predator-and-Prey/releases/tag/v2.0.0)

## Installing 

``` bash
# install
make

# run
make run

#clean-up
make clean
```

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details
