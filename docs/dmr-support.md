# DMR Resource Support

## Overview

DMFSI now includes DMOD Resource File (DMR) support for easy installation and packaging of the module along with its documentation, headers, and examples.

## What is a DMR File?

A `.dmr` (DMOD Resource File) is a declarative configuration file that specifies which files and directories from a package should be installed and where they should be placed. It supports environment variable substitution and allows for flexible resource management.

## Available Resources

When you install DMFSI using `dmf-get`, you can choose which resources to install:

### Full Installation (Default)

```bash
dmf-get dmfsi
```

Installs everything:
- ✓ DMF module file (`dmfsi.dmf`)
- ✓ Header files (`inc/dmfsi.h` and generated `dmfsi_defs.h`)
- ✓ Documentation (API reference, getting started guide)
- ✓ README and license files
- ✓ Implementation notes

### Minimal Installation

```bash
dmf-get --mini dmfsi
```

Installs only:
- ✓ DMF module file (`dmfsi.dmf`)

### Headers Only

```bash
dmf-get headers dmfsi
```

Installs only:
- ✓ Header files (`inc/dmfsi.h` and `dmfsi_defs.h`)

Useful for development when you need to implement DMFSI in your own module.

### Documentation Only

```bash
dmf-get docs dmfsi
```

Installs only:
- ✓ Documentation files (API reference, getting started guide)

Useful for offline documentation access or integration with `dmf-man`.

## RamFS Example

The RamFS example module also has DMR support:

### Full Installation

```bash
dmf-get ramfs
```

Installs:
- ✓ RamFS module file (`ramfs.dmf`)
- ✓ README and documentation
- ✓ Source code (`ramfs.c`)
- ✓ Build files (Makefile, CMakeLists.txt)

### Minimal Installation

```bash
dmf-get --mini ramfs
```

Installs only:
- ✓ RamFS module file (`ramfs.dmf`)

## DMR File Locations

- **Main module**: `dmfsi.dmr` (repository root)
- **RamFS example**: `examples/ramfs/ramfs.dmr`

## Resource Mappings

The DMFSI DMR file defines the following resource mappings:

| Resource Key | Source | Destination |
|--------------|--------|-------------|
| `dmf` | `./dmfsi.dmf` | `${DMOD_DMF_DIR}/${module}.dmf` |
| `dmfc` | `./dmfsi.dmfc` | `${DMOD_DMF_DIR}/${module}.dmfc` |
| `dmd` | `./dmfsi.dmd` | `${destination}/${module}.dmd` |
| `docs` | `./docs` | `${destination}/${module}/docs` |
| `readme` | `./README.md` | `${destination}/${module}/README.md` |
| `implementation` | `./IMPLEMENTATION.md` | `${destination}/${module}/IMPLEMENTATION.md` |
| `inc` | `./inc` | `${destination}/${module}/inc` |
| `license` | `./LICENSE` | `${destination}/${module}/LICENSE` |

## Environment Variables

The DMR files use the following special variables:

- `${DMOD_DMF_DIR}` - Default directory for DMF modules
- `${destination}` - Installation destination (from `-o` flag or `DMOD_DMF_DIR`)
- `${module}` - The module name (`dmfsi` or `ramfs`)

## Creating Packages

When creating a package for distribution:

1. Include the `.dmr` file in the root of your zip package
2. Structure your package to match the source paths in the DMR file
3. The package name should match the module name (e.g., `dmfsi.zip`)

Example package structure:

```
dmfsi.zip
├── dmfsi.dmr          # Resource mapping file
├── dmfsi.dmf          # Module binary
├── docs/              # Documentation
│   ├── dmfsi-api.md
│   └── getting-started.md
├── inc/               # Headers
│   ├── dmfsi.h
│   └── dmfsi_defs.h
├── README.md
├── IMPLEMENTATION.md
└── LICENSE
```

## Usage Examples

### Install DMFSI with all resources

```bash
dmf-get dmfsi
# Installs to $DMOD_DMF_DIR/dmfsi/
```

### Install to custom location

```bash
dmf-get dmfsi -o /custom/path
# Installs to /custom/path/dmfsi/
```

### Extract headers for development

```bash
dmf-get headers dmfsi -o ./include
# Extracts headers to ./include/
```

### View documentation

```bash
dmf-get docs dmfsi
dmf-man dmfsi
# View module documentation in console
```

## Best Practices

When creating DMR files for your own DMFSI implementations:

1. **Always include DMF resources**: The `dmf` key is required
2. **Use generic paths**: Use `${module}` instead of hardcoded names
3. **Group related resources**: Use comments to organize resource entries
4. **Document resources**: Add comments explaining what each resource is
5. **Consider mini mode**: Mark essential resources appropriately
6. **Include documentation**: Provide markdown documentation for `dmf-man`
7. **Include headers**: Allow developers to use your module's API
8. **Include license**: Always include license information

## Related Documentation

- [DMR File Format Specification](https://github.com/choco-technologies/dmod/blob/develop/docs/dmr-file-format.md)
- [dmf-get Tool Documentation](https://github.com/choco-technologies/dmod/blob/develop/tools/system/dmf-get/README.md)
- [DMFSI API Reference](dmfsi-api.md)
- [Getting Started Guide](getting-started.md)

## Example: dmclk Module

For a complete example of a module with DMR support, see:
- https://github.com/choco-technologies/dmclk

This project demonstrates best practices for DMR file structure and resource organization.
