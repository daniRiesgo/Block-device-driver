# Block-device-driver



## Set up environment


1. Create folder mqueue:
`` mkdir /mqueue `` 
2. Add the following line to fstab:
``none /mqueue mqueue rw 0 0``
3. Mount the system
``mount -a``
4. Set permissions so that users can write and delete only their own data.
``chmod a+rwxt /mqueue``

## Credit

Some of the code exposed belongs to Universidad Carlos III de Madrid. Project developed by [Alejandro](https://github.com/alexrs95/), [Diego](https://github.com/diegovicen/) and [me](https://github.com/daniriesgo/).
