## BTM State

| Feature                     | Status          | Notes             |
|----------------------------|------------------|--------------------|
| Memory map gathering       | ✅               |                    |
| Framebuffer info gathering | ✅               |                    |
| PCI device info gathering  | ✅               |                    |
| Other device info gathering| ❌               | To do              |
| PE32+ header run support   | ✅               |                    |
| PE32 header run support    | ➖               | To finish          |
| ELF header run support     | ❌               | To do              |

## Kernel State

| Feature                     | Status | Notes                           |
|----------------------------|--------|----------------------------------|
| Physical memory allocation | ✅     |                                  |
| SATA/AHCI storage driver   | ➖     | Working basic commands, to extend|
| Disk HAL                   | ➖     | In progress                      |
| GPT handler                | ➖     | In progress (Main focus)         |
| MBR handler                | ➖     | To do                            |
| File system                | ❌     | Awaiting Disk management         |
| Framebuffer writing (CPU)  | ❌     | Awaiting Filesystem              |
| Simple debugger            | ❌     | Awaiting Filesystem              |
| Virtual memory allocation  | ❌     | Awaiting Debugger                |
| Multitasking               | ❌     | Awaiting Virtual memory          |
| Other necessary drivers    | ❌     | Awaiting all above               |
| Basic UI                   | ❌     | Awaiting all above               |
