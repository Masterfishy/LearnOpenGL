# Learning Open GL

This is my environment for following along to [this](https://learnopengl.com/Introduction) OpenGL tutorial.

## Building

These steps will push build artifacts to a `build-volume`. See [steps](#make-the-build-volume) for making the build volume.

**1. Build the development image**
```bash
podman image build -t webgl.dev -f Containerfile.dev .
```

**2. Run a container form the dev image**

This image is intended to be run with the Dev Container VSCode extension (for now).

**3. Build the code**
```bash
mkdir build
cd build
emcmake cmake ..
emmake make
```

## Hosting

This image will host artifacts found in the `build-volume` with hot reloading. See [steps](#make-the-build-volume) for making the build volume.

**1. Build the development hosting image**

```bash
podman image build -t nginx.hosting -f Containerfile.dev-hosting .
```

**2. Run a container the dev hosting image**

```bash
podman run --replace --name learnOpenGL.hosting --network host --volume build-volume:/usr/share/nginx/html:ro nginx.hosting
```

## Make the `build-volume`

**1. Create a build volume**
```bash
podman volume create build-volume
```
