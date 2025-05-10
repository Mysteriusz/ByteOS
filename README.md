## BTM State

| Feature                     | Status          | Notes             | Timeline        |
|----------------------------|------------------|--------------------|------------------|
| Memory map gathering       | ✅               |                    | Done             |
| Framebuffer info gathering | ✅               |                    | Done             |
| PCI device info gathering  | ✅               |                    | Done             |
| Other device info gathering| ❌               | To do              | Future           |
| PE32+ header run support   | ✅               |                    | Done             |
| PE32 header run support    | ➖               | To finish          | Future           |
| ELF header run support     | ❌               | To do              | Future           |

## Kernel State

| Feature                     | Status | Notes                           | Timeline        |
|----------------------------|--------|----------------------------------|------------------|
| Physical memory allocation | ✅     |                                  | Done             |
| SATA/AHCI storage driver   | ➖     | Working basic commands           | Future           |
| Multi arch support         | ➖     | In progress (Main focus)         | ~ 1 Week         |
| Disk HAL                   | ➖     | In progress                      | ~ 2-3 Weeks      |
| GPT handler                | ➖     | In progress (Main focus)         | ~ 1-2 Weeks      |
| MBR handler                | ➖     | To do                            | Future           |
| File system                | ❌     | Awaiting Disk management         | ~ 1 Month        |  
| Framebuffer writing (CPU)  | ❌     | Awaiting Filesystem              | ~ 1 Month        |
| Simple debugger            | ❌     | Awaiting Filesystem              | ~ 1-2 Months     |
| Virtual memory allocation  | ❌     | Awaiting Debugger                | ~ 2 Months       |
| Multitasking               | ❌     | Awaiting Virtual memory          | ~ 3 Months       |
| Other necessary drivers    | ❌     | Awaiting all above               | ~ 3-4 Months       |
| Basic UI                   | ❌     | Awaiting all above               | ~ >3 Months   |
