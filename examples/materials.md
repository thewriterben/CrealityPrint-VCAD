# Material Configuration Examples

## Example 1: Basic Multi-Region Design
```json
{
  "region1": "PLA",
  "region2": "ABS",
  "support": "TPU"
}
```

## Example 2: Graded Lattice Structure
```json
{
  "core": "ABS",
  "outer_shell": "PETG",
  "flexible_joint": "TPU"
}
```

## Example 3: Soft Robotics Design
```json
{
  "rigid_frame": "ABS",
  "flexible_actuator": "TPU",
  "reinforcement": "PETG"
}
```

## Example 4: Impact-Absorbing Structure
```json
{
  "hard_surface": "ABS",
  "energy_absorber": "TPU",
  "base_structure": "PLA"
}
```

## Material Properties

| Material | Printing Temp | Bed Temp | Flexibility | Use Case |
|----------|---------------|----------|-------------|----------|
| PLA      | 200°C         | 60°C     | Rigid       | Base structures, general purpose |
| ABS      | 240°C         | 100°C    | Rigid       | High-temp regions, rigid frames |
| TPU      | 230°C         | 50°C     | Flexible    | Compliant mechanisms, soft regions |
| PETG     | 235°C         | 80°C     | Semi-flex   | Durable parts, reinforcements |
