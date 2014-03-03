#
#
# vi: set ft=ruby :

Vagrant.configure("1") do |config|
  config.vm.box       = 'precise64'
  config.vm.box_url   = '/Volumes/ERIC_DATA/vagrant/templates/precise64.box'
  config.vm.host_name = 'ts-dev-box'

  config.vm.network :hostonly, "192.168.100.10"
  config.vm.forward_port 8080, 8080, auto_correct: true

  config.vm.provision :puppet,
    :manifests_path => 'puppet/manifests'
end

Vagrant.configure("2") do |config|
    config.vm.provider :virtualbox do |vb|
        vb.customize ["modifyvm", :id, "--memory", "1024"]
    end
    config.vm.synced_folder ".", "/src/ats", :nfs => true
end
