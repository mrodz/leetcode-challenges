import java.util.HashMap;

class Solution {
    public int[] twoSum(int[] nums, int target) {
        HashMap<Integer, Integer> differences = new HashMap<>();

        for (int i = 0; i < nums.length; i++) {
            if (differences.containsKey(nums[i])) {
                return new int[] { differences.get(nums[i]), i };
            }
            differences.put(target - nums[i], i);
        }
        
        return null;
    }
}