# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  # Don't check for Vagrant base box updates automatically
  config.vm.box_check_update = false

  # Forward ports
  config.vm.network "forwarded_port", guest: 22, host: 2222
  config.vm.network "forwarded_port", guest: 3724, host: 3724
  config.vm.network "forwarded_port", guest: 8129, host: 8129

  config.vm.provider "virtualbox" do |vb|
    vb.gui = true
	vb.cpus = 1
    vb.memory = "4096"
  end
  
  # Provision the virtual machine with shell commands
  config.vm.provision "shell", path: "Vagrant_provision.sh"
end
