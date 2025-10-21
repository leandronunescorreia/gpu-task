# Contributing to GPU-Task

Thank you for your interest in contributing to GPU-Task!

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR_USERNAME/gpu-task.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes: `./build.sh && cd build && ./device_info`
6. Commit your changes: `git commit -am 'Add some feature'`
7. Push to the branch: `git push origin feature/your-feature-name`
8. Submit a pull request

## Development Setup

### Prerequisites
- CMake 3.10+
- C++17 compatible compiler
- OpenCL SDK (optional for development, recommended for testing)

### Building
```bash
./build.sh
```

### Testing
```bash
cd build
./device_info
./vector_add
./scalar_multiply
```

## Code Style

- Use consistent indentation (4 spaces)
- Follow existing code style
- Add comments for complex logic
- Keep functions focused and single-purpose

## Areas for Contribution

- **Additional backends**: CUDA, Vulkan, Metal
- **More examples**: Matrix operations, image processing, etc.
- **Performance improvements**: Memory management, async operations
- **Documentation**: Improve README, add tutorials
- **Testing**: Unit tests, integration tests
- **Bug fixes**: Check issues for known bugs

## Submitting Changes

- Write clear commit messages
- Reference any related issues
- Ensure code compiles without warnings
- Test on multiple platforms if possible
- Update documentation as needed

## Questions?

Feel free to open an issue for:
- Feature requests
- Bug reports
- Questions about the code
- General discussion

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
