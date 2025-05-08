# Learning Open GL

This is my environment for following along to [this](https://learnopengl.com/Introduction) OpenGL tutorial.

## Build the development image

```
podman image build -t webgl.dev -f .\Containerfile.dev .
```

This image is intended to be used with the VSCode devcontainer extension.
