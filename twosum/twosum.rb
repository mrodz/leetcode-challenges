# @param {Integer[]} nums
# @param {Integer} target
# @return {Integer[]}
def two_sum(nums, target)
    differences = Hash.new

    for i in 0...nums.length do
        diff = target - nums[i]
        if differences.key?(nums[i])
            return [differences[nums[i]], i];
        end
        differences[diff] = i
    end
end