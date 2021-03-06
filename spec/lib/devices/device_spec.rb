require 'spec_helper'

DEVICE = RubiCL::Device

describe DEVICE do
  it 'is abstract' do
    expect { DEVICE.new }.to raise_error
  end

  it 'defines transferring an object to device memory' do
    DEVICE.instance_methods.include?(:load_object).should be true
  end

  it 'defines retrieving an integer array from device memory' do
    DEVICE.instance_methods.include?(:retrieve_integers).should be true
  end

  context '#map' do
    it 'is defined' do
      DEVICE.instance_methods.include?(:map).should be true
    end

    it 'causes a task to be queued' do
      class SomeDEVICE < DEVICE; end
      device = SomeDEVICE.new

      device.instance_eval { @buffer.instance_eval { @buffer_type = :int } }
      expect { device.map { |i| i + 1 } }.to_not raise_error
      device.instance_eval { @task_queue }.size.should be 1

      work_unit = device.instance_eval { @task_queue }.shift
      work_unit.statements.should == ['x = x + 1']
    end
  end

  it 'creates a task queue when initialized' do
    class SomeDevice < DEVICE
    end
    SomeDevice.new.instance_variable_get(:@task_queue).should_not be nil
  end

  it "allows the output to be retrieved by 'casting' to a Ruby type" do
    class StubDevice < DEVICE
      def retrieve_integers
        [1]
      end
    end
    StubDevice.new[Fixnum].should == [1]
  end

  it 'caches the loaded dataset when no mutating changes are made' do
    RubiCL::CPU.get.should_receive(:retrieve_pinned_integer_dataset_from_buffer).never
    RubiCL::CPU.get.load_object(:int, [1, 2, 3])[Fixnum].should == [1, 2, 3]
  end

  it 'caches the retrieved dataset when no mutating changes are made' do
    RubiCL::CPU.get.instance_eval { @buffer }.should_receive(:retrieve_pinned_integer_dataset_from_buffer).once.and_return([2, 3, 4])
    cpu = RubiCL::CPU.get.load_object(:int, [1, 2, 3]).map { |x| x + 1 }
    2.times { cpu.retrieve_integers.should == [2, 3, 4] }
  end

  it 'can compute the summation of an integer buffer' do
    RubiCL::CPU.get.load_object :int, [1, 2, 3, 4]
    RubiCL::CPU.get.sum.should == 10
  end

  it 'can count the number of occurrences of a number in an integer buffer' do
    RubiCL::CPU.get.load_object :int, [1, 1, 2, 2, 2]
    RubiCL::CPU.get.count(2).should == 3
  end

end
