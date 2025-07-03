# TODO

## Tech Debt

### Decoupling

1. Decouple Texture and Ant so that Ant may be unit tested without Raylib opening a window.
2. Decouple Textures and World so that world may be unit tested without Raylib opening a window
3. Decouple Raylib and game update so that the core game functionality may be tested without Raylib opening a window

### Unit Tests
1. Unit test Surroundings Encoding
2. Unit test Brain
3. Unit test Genome Breeding
4. Ant Initailization
5. Ant Life Cycle
6. Population mean.  


### Simplification
1. Get a Random Vector2 written to simplify testing, maximize reuse
2. Move all textures to the texture cache and make everything get textures from there.

## Potential Bugs
### Food
1. Food Count should never be zero.

## Features
### Resources
1. Add support for multiple resources
### Food
1. When two ants arrive at food at the same time, they should consume the food equally.
### Water
1. Implement water as a second resource.

### Analytics
1. Draw a graph of the population fitness over time.
2. When an ant is clicked on in game mode, open a window that shows their brain.
3. In the Ant's brain window, allow the user to toggle seeing what the ant is seeing.
4. Show a 'score' in terms of population 

### Control
1. Allow the user to modify the time step so that time may be accelerated.
2. Allow the user to modify the ant's starting energy level.
3. Allow the user to modify the number of layers in the ant's neural network.
4. Allow the user to modify the neuron count per layer.

### Styling
1. Get a better font in multiple sizes

### Species
1. Add support for more than one species
#### Spiders
1. Implement Spiders
2. Spiders eat ants, ants flee spiders (or should)