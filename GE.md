# GameplayEffect


# 配置选项
## Duration Policy （持续时间策略）：
    决定 Gameplay Effect 的持续时间类型，可能的选项包括：
1. Instant：立即生效且瞬时完成（如一次性伤害）。
2. Infinite：效果持续直到被手动移除（如持续状态效果）。
3. Has Duration：效果会持续指定时间。（如果选择 Has Duration，需要设置持续时间的数值或使用其他逻辑决定）

# Gameplay Effect
1. Components
    1. 用于添加其他模块化功能，例如基于标签的触发行为。
    2. 这些组件通常扩展或调整基础的 Gameplay Effect 功能。

2. Modifiers（属性修饰器）: 定义修改目标属性的逻辑。例如：
    1. 增加或减少特定属性（如生命值、攻击力）。
    2. 可以设置具体的修改值、计算公式或依赖条件。
    Attribute：目标属性（如 Health、AttackPower）。
    Modifier Op：操作类型：
        Add：直接相加（CurrentValue += X）。
        Multiply：按当前值相乘（*CurrentValue = X）。
        Override：覆盖当前值（CurrentValue = X）。
    Magnitude：修改量，可以是固定值、基于属性或自定义计算（如 Formula = 0.1 * Target.MaxHealth）。
    Modifier Tag：触发条件标签（例如仅当目标有 Burning 标签时生效）

3. Modifier 通常结合 Attributes（属性系统）使用，影响玩家或AI的数值。

4. Executions（执行逻辑）
    1. 执行更复杂的逻辑（如动态计算伤害）。
    2. 与 Modifiers 不同，Executions 通常用于需要即时运行的复杂公式，例如攻击力和护甲的乘除关系
关键参数：
Calculation Class：继承 UGameplayEffectExecutionCalculation 的类，用于实现复杂的数值逻辑（例如根据防御力计算伤害）。
Passed In Tags：传递给执行逻辑的标签，用于条件判断。

# Gameplay Cues

1. Require Modifier Success to Trigger Cues
    1. 只有在 Modifier 成功应用后，才会触发关联的 Gameplay Cue。
    2. 用于确保视觉或声音效果（Gameplay Cue）与属性改变同步。

2. Suppress Stacking Cues
    1. 防止同一 Gameplay Cue 堆叠触发。
    2. 例如，如果一个效果重复应用，该选项可以抑制重复播放的视觉/音效反馈。

3. Gameplay Cues
    1. 定义关联的 Gameplay Cue，这些通常是视觉效果、音效或粒子效果。
    2. Gameplay Cue 的作用是增强游戏反馈，例如中毒效果的绿色光环或击中敌人的火花效果。

# Stacking Type
1. 定义效果的堆叠规则，常见选项有：
    1. None：不堆叠，效果只会存在一次。
    2. Aggregate By Source：来自同一来源的效果堆叠;按施加者分开堆叠（例如多个玩家施加的减速效果独立计算）。
    3. Aggregate By Target：效果在目标上堆叠，但来源可能不同;所有施加者的效果合并堆叠。
Stack Limit Count：最大堆叠层数。
Stack Duration Refresh Policy：叠加时刷新持续时间的方式（例如重置或延长）。
Stack Expiration Policy：层数过期时的处理（例如移除最旧或最新层）。

如果选择堆叠，还需要配置堆叠的逻辑，例如堆叠次数上限和叠加方式（叠加增加还是覆盖旧值）。


# Tags（标签系统）
作用：通过标签控制效果的交互逻辑。
关键标签组：
    Asset Tags：效果自身的标签（用于标识效果类型）。
    Granted Tags：应用时赋予目标的标签（例如 Burning）。
    Ongoing Tag Requirements：效果持续期间需要的标签条件（若目标失去标签，效果会移除）。
    Application Tag Requirements：应用效果所需的标签条件（例如目标必须有 Frozen 标签才能生效）

#  Application Requirements（应用条件）
作用：定义效果能否被应用的自定义逻辑。
实现方式：
    继承 UGameplayEffectCustomApplicationRequirement 类，实现 CanApplyGameplayEffect 函数。
    例如：仅当目标生命值低于50%时才能应用效果。


# Period（周期触发）
作用：周期性触发效果（例如每2秒造成一次伤害）。
关键参数：
    Period：触发间隔时间。
    Execute on Application：首次应用时立即触发一次。
    Infinite Effect：仅在 Duration Policy = Infinite 时可用，周期性无限触发。




SendGameplayEventToActor(Actor, EventTag, Payload)

FGameplayEventData Payload;
Payload.EventMagnitude = 200.0f;   // 这次想加 200 血

UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
    PlayerCharacter,
    FGameplayTag::RequestGameplayTag("Event.HealthPotion"),  // 自定义事件标签
    Payload
);